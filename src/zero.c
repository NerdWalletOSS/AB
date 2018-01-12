#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "ab_constants.h"
#include "ab_types.h"
#include "ab_globals.h"
#include "macros.h"
#include "zero.h"
#include "auxil.h"


//<hdr>
int
zero_test(
    uint32_t test_idx
    )
{
  int status = 0;
  if ( test_idx > AB_MAX_NUM_TESTS ) { go_BYE(-1); }
  TEST_META_TYPE *ptr_test = &(g_tests[test_idx]);

  memset(ptr_test->name, '\0', AB_MAX_LEN_TEST_NAME+1);
  ptr_test->test_type = 0;
  ptr_test->x_tst_id = 0;
  ptr_test->name_hash = 0;
  ptr_test-> external_id = 0;
  ptr_test-> has_filters = false;
  ptr_test-> is_dev_specific = false;
  ptr_test->state = 0;
  ptr_test->seed = 0;

  for ( uint32_t v = 0; v < ptr_test->num_variants; v++ ) { 
    free_if_non_null(ptr_test->variants->custom_data);
    free_if_non_null(ptr_test->variants->url);
  }
  free_if_non_null(ptr_test->variants);
  ptr_test->num_variants = 0;

  ptr_test->final_variant_id = UINT_MAX;
  ptr_test->final_variant_idx = UINT_MAX;
BYE:
  return status;
}
//<hdr>
int 
free_globals(
    void
    )
//</hdr>
{
  int status  = 0;
  // TODO P2 Uncomment; free_if_non_null(g_devices);  g_n_devices = 0;

  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    status = zero_test(i); cBYE(status);
  }
  free_if_non_null(g_ss_response);  g_sz_ss_response = 0;

  free_if_non_null(g_uuid);
  free_if_non_null(g_log_q);
  //------------------------------------------------
  shutdown_curl();
  if ( g_statsd_link != NULL ) { 
    statsd_finalize(g_statsd_link);
    g_statsd_link = NULL;
  }
  // For Lua
  if ( g_L != NULL ) { lua_close(g_L); g_L = NULL; }
BYE:
  return status;
}

void 
shutdown_curl(
    void
    )
{
  if ( g_ch != NULL ) { 
    curl_easy_cleanup(g_ch);  g_ch = NULL;
  }
  if ( g_curl_hdrs != NULL ) { 
    curl_slist_free_all(g_curl_hdrs); g_curl_hdrs = NULL;
  }
  if ( g_ss_ch != NULL ) { 
    curl_easy_cleanup(g_ss_ch);  g_ss_ch = NULL;
  }
  if ( g_ss_curl_hdrs != NULL ) { 
    curl_slist_free_all(g_ss_curl_hdrs); g_ss_curl_hdrs = NULL;
  }
  curl_global_cleanup();  
}
