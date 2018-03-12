#include "ab_incs.h"
#include "ab_types.h"
#include "ab_globals.h"
#include "aux_zero.h"
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
  ptr_test->id = 0;
  ptr_test->name_hash = 0;
  ptr_test-> external_id = 0;
  ptr_test-> has_filters = false;
  ptr_test-> is_dev_specific = false;
  ptr_test->state = 0;
  ptr_test->seed = 0;

  for ( uint32_t v = 0; v < ptr_test->num_variants; v++ ) { 
    free_if_non_null((ptr_test->variants[v].custom_data));
    free_if_non_null((ptr_test->variants[v].url));
  }
  free_if_non_null(ptr_test->variants);
  ptr_test->num_variants = 0;

  ptr_test->final_variant_id = NULL;
  ptr_test->final_variant_idx = NULL;
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
  curl_global_cleanup();   // TODO P4 Check that this is ok at startup 
}
