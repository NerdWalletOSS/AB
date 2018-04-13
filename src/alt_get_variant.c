#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "ab_auxil.h"
#include "get_test_idx.h"
#include "add_test.h"
#include "alt_get_variant.h"
#include "get_variant.h"
#include "dump_log.h"
#include "make_guid.h"

//<hdr>
static int
find_test(
    const char *args, 
    int *ptr_test_idx
    )
//</hdr>
{
  int status = 0;
  *ptr_test_idx = -1;
  int test_idx = -1;
  char test_name[AB_MAX_LEN_TEST_NAME+1]; 
  uint64_t external_id = 0;
  // Test can be specified by name or external_id 
  memset(test_name, '\0', (AB_MAX_LEN_TEST_NAME+1));
  status = extract_name_value(args, "TestName=", '&', test_name, 
      AB_MAX_LEN_TEST_NAME); 
  cBYE(status);
  if ( test_name[0] == '\0' ) {
    char buf[32]; memset(buf, '\0', 32);
    status = extract_name_value(args, "ExternalID=", '&', buf, 32); 
    // DELETED DEC 2016 cBYE(status);
    if ( ( status != 0 ) || ( *buf == '\0' ) ) { 
      status = extract_name_value(args, "nw_campaign_id=", '&', buf, 32); 
      cBYE(status);
    }
    if ( *buf == '\0' ) { go_BYE(-1); } 
    int64_t tempI8;
    status = stoI8(buf, &tempI8); 
    if ( status < 0 ) { 
      fprintf(stderr, "buf = [%s] \n", buf);
    }
    cBYE(status);
    if ( tempI8 <= 0 ) { go_BYE(-1); }
    external_id = (uint64_t) tempI8;
    for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) {
      if ( g_tests[i].external_id == external_id ) {
        test_idx = i; break;
      }
    }
  }
  else {
    status = get_test_idx(test_name, AB_TEST_TYPE_XY, &test_idx);
    cBYE(status);
  }
  // Verify test exists and is of type XYTest
  if ( g_tests[test_idx].test_type != AB_TEST_TYPE_XY ) { go_BYE(-1); }
  *ptr_test_idx = test_idx;
BYE:
  return status;
}

//<hdr>
int alt_get_variant(
    const char *args
    )
//</hdr>
{
  int status = 0;
  int test_idx = -1;
  uint64_t curr_time = RDTSC();
  char in_tracer[AB_MAX_LEN_TRACER+1];
  char out_tracer[AB_MAX_LEN_TRACER+1];
  uint32_t variant_idx, variant_id;

  if ( ( args == NULL ) || ( *args == '\0' ) ) { go_BYE(-1); }
  memset(g_redirect_url, '\0', (AB_MAX_LEN_REDIRECT_URL+1));
  memset(in_tracer, '\0', AB_MAX_LEN_TRACER+1);
  memset(out_tracer, '\0', AB_MAX_LEN_TRACER+1);

  status = find_test(args, &test_idx);cBYE(status);
  if ( test_idx < 0 ) { go_BYE(-1); }
  uint32_t test_id = g_tests[test_idx].id;
  g_log_get_alt_variant_calls++; // increment only if test is legit 
  get_tracer(args, in_tracer); 
  set_tracer(out_tracer, AB_MAX_LEN_TRACER); 

  int device_idx;
  int bin = RDTSC() % AB_NUM_BINS;
  if ( g_tests[test_idx].is_dev_specific ) {
    // TODO P3 Delete later. Following is temporary code to aid debugging
    // We change g_device_idx if (1) GET param Device sent (2) it is valid
    if ( g_justin_cat_id == 0 ) { 
      g_justin_cat_id = g_justin_cat_other_id; 
    }
    device_idx = g_justin_cat_id - 1;
    char buf[16]; memset(buf, '\0', 16); 
    status = extract_name_value(args, "Device=", '&', buf, 16);
    if ( *buf != '\0' ) { 
      for ( int j = 0; j < g_n_justin_cat_lkp; j++ ) { 
        if ( strcmp(g_justin_cat_lkp[j].name, buf) == 0 ) { 
          device_idx = j; break;
        }
      }
    }
  }
  else {
    device_idx = 0; 
  }
  if ( device_idx >= g_n_justin_cat_lkp ) { go_BYE(-1); }
  //----------------------------------
  if ( g_tests[test_idx].state == TEST_STATE_TERMINATED ) {
    variant_idx = g_tests[test_idx].final_variant_idx[device_idx];
    variant_id  = g_tests[test_idx].final_variant_id[device_idx];
  }
  else if ( g_tests[test_idx].state == TEST_STATE_STARTED ) {
    variant_idx = g_tests[test_idx].variant_per_bin[device_idx][bin];
    variant_id  = 0; // TODO FIX P0
  }
  else {
    go_BYE(-1);
  }
  const char *url = g_tests[test_idx].variants[variant_idx].url;
  char *var_name = g_tests[test_idx].variants[variant_idx].name;
  //----------------------------------
  sprintf(g_rslt, "{ \"Variant\" : \"%s\", \"GUID\" : \"%s\", \"VariantID\" :  %d, \"URL\" : \"%s\" }",
      var_name, g_out_tracer, variant_id, url);
  // START: Handle fact that question mark should occur only once
  char separator = '?';
  for ( const char *cptr = g_tests[test_idx].variants[variant_idx].url; 
      *cptr != '\0'; cptr++ ) { 
    if ( *cptr == '?' ) { separator = '&'; break; }
  }
  if ( *args == '?' ) { args++; }
  // STOP: Handle qmark 

  int nw = snprintf(g_redirect_url, AB_MAX_LEN_REDIRECT_URL,
      "%s%c%s&VariantID=%uGUID=%s", 
      g_tests[test_idx].variants[variant_idx].url, 
      separator, 
      args, 
      variant_id,
      g_out_tracer);
  if ( nw >= AB_MAX_LEN_REDIRECT_URL ) { 
    memset(g_redirect_url, '\0', (AB_MAX_LEN_REDIRECT_URL+1));
    status = -1;
  }

  PAYLOAD_TYPE lcl_payload;
  memset(&lcl_payload, '\0', sizeof(PAYLOAD_TYPE));
  strcpy(lcl_payload.out_tracer, g_out_tracer);
  lcl_payload.time       = curr_time;
  lcl_payload.test_id    = test_id;
  lcl_payload.variant_id = variant_id;

  status = log_decision(lcl_payload); cBYE(status);
  cBYE(status);
  // TODO: P4 Do we need trailing slash afterlanding page URL?
BYE:
  return status;
}

/*
Yes. The "query string" part of the URL always starts with a `?` and uses `&` to separate arguments, so .com?something=true&something_else=false is valid but .com&something=true&something_else=false and .com?something=true?something_else=false are invalid.

Typically you would lean on a package or write your own to parse a URL into it's pieces, operate on the pieces as native types like arrays and objects, then reassemble the URL when you're done. For example, in PHP the function http_build_query http://php.net/manual/en/function.http-build-query.php does exactly this, taking an associative array of keys and values and transforming them into a valid query string.


*/
