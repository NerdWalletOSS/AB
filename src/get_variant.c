#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "get_test_idx.h"
#include "log_decision.h"
#include "chk_exclude.h"
#include "get_variant.h"



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
  memset(g_uuid, '\0', g_cfg.uuid_len+1);
  //-------------------------------------------------------------
  status = get_test_name(args,  test_name);  cBYE(status);
  status = get_test_idx(test_name, test_type, &test_idx); cBYE(status);
  T = &(g_tests[test_idx]);
  g_log_get_variant_calls++; 
  // Extract UUID
  status = extract_name_value(args, "UUID=", '&', g_uuid, g_cfg.uuid_len);
  if ( ( status < 0 ) || ( *g_uuid == '\0' ) ) { 
    status = -1;
    g_log_no_uuid++;
  }
  cBYE(status);
  status = chk_uuid(g_uuid, g_cfg.uuid_len); cBYE(status);
  if ( status < 0 ) {
    g_log_bad_uuid++;
  }
  get_tracer(args, in_tracer);
  set_tracer(out_tracer, AB_MAX_LEN_TRACER);
  //--------------------------------------------------------
  // Deal with exclusions for categorical attributes
  int is_exclude = FALSE; int nw;
  if ( T->has_filters ) {  // ask Session Service
    status = chk_exclude(test_name, g_uuid, &is_exclude);
    if ( is_exclude == TRUE ) { 
      nw = snprintf(g_rslt, AB_MAX_LEN_RESULT,
          "{ \"Variant\" : \"Ineligible\", \"VariantID\" :  0, \"Test\" : \"%s\", \"TestID\" : %d }",
          test_name, 
          g_tests[test_idx].id);
      if ( nw >= AB_MAX_LEN_RESULT ) { go_BYE(-1); }
      goto BYE;
    }
  }
  // Deal with special case of Terminated test 
  if ( g_tests[test_idx].state == TEST_STATE_TERMINATED ) {
    uint32_t final_variant_idx = g_tests[test_idx].final_variant_idx[0];
    const char *cd = g_tests[test_idx].variants[final_variant_idx].custom_data;
    if ( cd == NULL ) { cd = "null"; }
    nw = snprintf(g_rslt, AB_MAX_LEN_RESULT, "{ \"Variant\" : \"%s\", \"VariantID\" :  %d, \"CustomData\" : %s, \"Test\" : \"%s\", \"TestID\" : %d  }",
        g_tests[test_idx].variants[final_variant_idx].name,
        g_tests[test_idx].variants[final_variant_idx].id,
        cd,
        test_name, 
        g_tests[test_idx].id);
    if ( nw >= AB_MAX_LEN_RESULT ) { go_BYE(-1); }
    goto BYE;
  }
  //-----------------------------------------------------------
  if ( g_tests[test_idx].state != TEST_STATE_STARTED )  { go_BYE(-1); }
  uint64_t uuid_hash = 
    spooky_hash64(g_uuid, g_cfg.uuid_len, g_tests[test_idx].seed);
  //-----------------------------------------------------------
  uint32_t bin = uuid_hash % AB_NUM_BINS;
  uint32_t variant_idx = g_tests[test_idx].variant_per_bin[0][bin];
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
      g_tests[test_idx].id);
  if ( nw >= AB_MAX_LEN_RESULT ) { go_BYE(-1); }
  //------------------------------------------------
  PAYLOAD_TYPE lcl_payload;
  memset(lcl_payload.uuid, '\0', AB_MAX_LEN_UUID+1);
  strcpy(lcl_payload.uuid, g_uuid);
  strcpy(lcl_payload.in_tracer,  in_tracer);
  strcpy(lcl_payload.out_tracer, out_tracer);
  lcl_payload.test_id    = g_tests[test_idx].id;
  lcl_payload.variant_id = variant_id;
  lcl_payload.time       = g_t_start;
  status = log_decision(lcl_payload);
  cBYE(status);
  //--------------------------------------------------------
BYE:
  return status;
}

