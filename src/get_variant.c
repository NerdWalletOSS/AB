#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <unistd.h>
#include "ab_constants.h"
#include "macros.h"
#include "ab_types.h"
#include "auxil.h"
#include "get_test_idx.h"
#include "add_test.h"
#include "get_variant.h"
#include "post_url.h"
#include "statsd-client.h"
#include "dump_log.h"

#include "ab_globals.h"

int 
get_variant(
    const char *args
    )
{
  int status = 0;
  char test_name[AB_MAX_LEN_TEST_NAME+1]; 
  int test_idx = -1;
  int test_type = AB_TEST_TYPE_AB;
  TEST_META_TYPE *T = NULL;
  char in_tracer[AB_MAX_LEN_TRACER+1];
  char out_tracer[AB_MAX_LEN_TRACER+1];

  if ( ( args == NULL ) || ( *args == '\0' ) ) { go_BYE(-1); }
  memset(g_uuid, '\0', g_uuid_len+1);
  //-------------------------------------------------------------
  status = get_test_name(args,  test_name);  cBYE(status);
  // TODO: log missing test_name 
  status = get_test_idx(test_name, test_type, &test_idx); cBYE(status);
  // TODO: log missing test
  T = &(g_tests[test_idx]);
  g_num_get_variant_calls++; 
  uint64_t t_start = get_time_usec();
  // Extract UUID
  status = extract_name_value(args, "UUID=", '&', g_uuid, g_uuid_len);
  cBYE(status);
  if ( ( status < 0 ) || ( *g_uuid == '\0' ) ) { go_BYE(-1); }
  // TODO: Log missing UUID
  status = chk_uuid(g_uuid, g_uuid_len, g_test_uuid_len); cBYE(status);
  // TODO: Log bad UUID
  get_tracer(args, in_tracer);
  set_tracer(out_tracer);
  //--------------------------------------------------------
  // Deal with exclusions for categorical attributes
  bool is_exclude = false; int nw;
  if ( T->has_filters ) {  // ask Session Service
    // TODO status = lua_chk_exclude(test_name); 
    if ( status < 0 ) { /* log bad filter call */  }
    if ( is_exclude ) { 
      nw = snprintf(g_rslt, AB_MAX_LEN_RESULT,
          "{ \"Variant\" : \"Ineligible\", \"VariantID\" :  0, \"Test\" : \"%s\", \"TestID\" : %d }",
          test_name, 
          g_tests[test_idx].x_tst_id);
      if ( nw >= AB_MAX_LEN_RESULT ) { go_BYE(-1); }
      goto BYE;
    }
  }
  // Deal with special case of Terminated test 
  if ( g_tests[test_idx].state == TEST_STATE_TERMINATED ) {
    uint32_t final_variant_idx = g_tests[test_idx].final_variant_idx;
    const char *cd = g_tests[test_idx].variants[final_variant_idx].custom_data;
    if ( cd == NULL ) { cd = "null"; }
    nw = snprintf(g_rslt, AB_MAX_LEN_RESULT, "{ \"Variant\" : \"%s\", \"VariantID\" :  %d, \"CustomData\" : %s, \"Test\" : \"%s\", \"TestID\" : %d  }",
        g_tests[test_idx].variants[final_variant_idx].name,
        g_tests[test_idx].variants[final_variant_idx].id,
        cd,
        test_name, 
        g_tests[test_idx].x_tst_id);
    if ( nw >= AB_MAX_LEN_RESULT ) { go_BYE(-1); }
    goto BYE;
  }
  //-----------------------------------------------------------
  if ( g_tests[test_idx].state != TEST_STATE_STARTED )  { go_BYE(-1); }
  uint64_t uuid_hash = 
    spooky_hash64(g_uuid, g_uuid_len, g_tests[test_idx].seed);
  //-----------------------------------------------------------
  uint16_t bin = uuid_hash % AB_NUM_BINS;
  uint32_t variant_idx = g_tests[test_idx].variant_per_bin[bin];
  if ( variant_idx >= g_tests[test_idx].num_variants ) { go_BYE(-1); }
  uint32_t variant_id  = g_tests[test_idx].variants[variant_idx].id;
  const char *cptr = g_tests[test_idx].variants[variant_idx].custom_data;
  if ( cptr == NULL ) { cptr = "null"; }
  nw = snprintf(g_rslt, AB_MAX_LEN_RESULT, 
      "{ \"Variant\" : \"%s\", \"VariantID\" : %d, \"CustomData\" : %s, \"Test\" : \"%s\", \"TestID\" : %d }", 
      g_tests[test_idx].variants[variant_idx].name, 
      variant_id, 
      cptr, 
      test_name, 
      g_tests[test_idx].x_tst_id);
  if ( nw >= AB_MAX_LEN_RESULT ) { go_BYE(-1); }
  //------------------------------------------------
  PAYLOAD_TYPE lcl_payload;
  memset(lcl_payload.uuid, '\0', AB_MAX_LEN_UUID+1);
  strcpy(lcl_payload.uuid, g_uuid);
  strcpy(lcl_payload.in_tracer,  in_tracer);
  strcpy(lcl_payload.out_tracer, out_tracer);
  lcl_payload.test_id    = g_tests[test_idx].x_tst_id;
  lcl_payload.variant_id = variant_id;
  lcl_payload.time       = t_start;
  status = log_decision(lcl_payload);
  cBYE(status);
  //--------------------------------------------------------
BYE:
  return status;
}

int
log_decision(
    PAYLOAD_TYPE lcl_payload
    )
{
  int status = 0;
  bool is_wait = false;
  if ( g_sz_log_q > 0  ) {
    pthread_mutex_lock(&g_mutex);	/* protect buffer */
    if ( g_n_log_q == g_sz_log_q ) { // TODO P3 Remove
      fprintf(stderr, "Waiting for space \n");
      is_wait = true;
    }
    while ( g_n_log_q == g_sz_log_q ) {
      /* If there is no space in the buffer, then wait */
      // fprintf(stderr, "producer waiting\n");
      pthread_cond_wait(&g_condp, &g_mutex);
    }
    if ( is_wait ) { fprintf(stderr, "Got space \n"); }

    int eff_wr_idx = g_q_wr_idx % g_sz_log_q;
    g_log_q[eff_wr_idx] = lcl_payload;
    g_q_wr_idx++; 
    g_n_log_q++;
    pthread_cond_signal(&g_condc);	/* wake up consumer */
    pthread_mutex_unlock(&g_mutex);	/* release the buffer */
  }
  else {
    memset(g_curl_payload,            '\0', AB_MAX_LEN_PAYLOAD+1);
    status = make_curl_payload(lcl_payload, g_curl_payload);
    status = post_url(g_ch, g_curl_payload, NULL);
    switch ( status ) {
      case 0  : /* all is well */ break;
      case -1 : go_BYE(-1); break;
      case -2 : /* Not mission critical failure */ status = 0; break; 
      default : go_BYE(-1); break;
    }
  }
BYE:
  return status;
}
