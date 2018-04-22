#include "ab_incs.h"
#include "ab_types.h"
#include "ab_globals.h"
#include "aux_zero.h"
#include "auxil.h"


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

  ptr_test->num_variants = 0;
  ptr_test->variants = NULL;

  ptr_test->final_variant_id = NULL;
  ptr_test->final_variant_idx = NULL;
  ptr_test->variant_per_bin = NULL;
BYE:
  return status;
}
//<hdr>
int
free_test(
    uint32_t test_idx
    )
{
  int status = 0;
  if ( test_idx > AB_MAX_NUM_TESTS ) { go_BYE(-1); }
  TEST_META_TYPE *ptr_test = &(g_tests[test_idx]);

  ptr_test->id = 0;
  ptr_test->name_hash = 0;
  ptr_test-> external_id = 0;
  ptr_test-> has_filters = false;
  ptr_test-> is_dev_specific = false;
  ptr_test->state = 0;
  ptr_test->seed = 0;

  for ( uint32_t v = 0; v < ptr_test->num_variants; v++ ) { 
    free_if_non_null(ptr_test->variants[v].custom_data);
    free_if_non_null(ptr_test->variants[v].url);
  }
  free_if_non_null(ptr_test->variants);
  ptr_test->num_variants = 0;

  free_if_non_null(ptr_test->final_variant_id);
  free_if_non_null(ptr_test->final_variant_idx);
  if ( ptr_test->variant_per_bin != NULL ) { 
    int num_devices = 0;
    if ( ptr_test->is_dev_specific ) {
      num_devices = g_n_justin_cat_lkp; 
    }
    else {
      num_devices = 1;  
    }
    for ( int i = 0; i < num_devices; i++ ) { 
      free_if_non_null(ptr_test->variant_per_bin[i]);
    }
  }
  free_if_non_null(ptr_test->variant_per_bin);
  memset(ptr_test->name, '\0', AB_MAX_LEN_TEST_NAME+1);
  ptr_test->test_type = 0;
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

int
malloc_test(
    int test_idx,
    int num_variants,
    int is_dev_specific
    )
{
  int status = 0;
  VARIANT_REC_TYPE *variants = NULL;
  TEST_META_TYPE *p = NULL;
  int num_devices;

  //-- Check parameters -------------------------
  if ( ( num_variants < AB_MIN_NUM_VARIANTS ) || 
       ( num_variants > AB_MAX_NUM_VARIANTS ) ) { 
    go_BYE(-1);
  }
  if ( ( is_dev_specific < 0 ) || ( is_dev_specific > 1 ) ) { 
    go_BYE(-1);
  }
  if ( ( test_idx < 0 ) || ( test_idx > AB_MAX_NUM_TESTS ) ) { 
    go_BYE(-1);
  }
  //-- Check slot where allocation is to be done
  p = &(g_tests[test_idx]);
  if ( p->name_hash != 0 ) { go_BYE(-1); }
  if ( p->id != 0 ) { go_BYE(-1); }
  if ( p->external_id != 0 ) { go_BYE(-1); }
  if ( p->state != 0 ) { go_BYE(-1); }
  if ( p->seed != 0 ) { go_BYE(-1); }
  if ( p->variants != NULL ) { go_BYE(-1); }
  if ( p->final_variant_id  != NULL ) { go_BYE(-1); }
  if ( p->final_variant_idx != NULL ) { go_BYE(-1); }
  //------------------------------------------
  variants = malloc(num_variants * sizeof(VARIANT_REC_TYPE));
  free_if_non_null(variants);
  for ( uint32_t v = 0; v < p->num_variants; v++ ) { 
    memset(variants, '\0', sizeof(VARIANT_REC_TYPE));
  }
  for ( uint32_t v = 0; v < p->num_variants; v++ ) { 
    variants[v].custom_data = malloc(AB_MAX_LEN_CUSTOM_DATA+1);
    return_if_malloc_failed(variants[v].custom_data);
    memset(variants[v].custom_data, '\0',  AB_MAX_LEN_CUSTOM_DATA+1);
    variants[v].url = malloc(AB_MAX_LEN_URL+1);
    return_if_malloc_failed(variants[v].url);
    memset(variants[v].url, '\0',  AB_MAX_LEN_URL+1);
  }
  p->variants = variants;
  p->num_variants = num_variants;
  //--- Determine number of devices
  if ( p->is_dev_specific ) {
    num_devices = g_n_justin_cat_lkp; 
  }
  else {
    num_devices = 1;  
  }
  if ( num_devices < 1 ) { go_BYE(-1); }
  //------------------------------------------
  p->final_variant_idx = malloc(num_devices * sizeof(uint32_t));
  return_if_malloc_failed(p->final_variant_idx);
  p->final_variant_id = malloc(num_devices * sizeof(uint32_t));
  return_if_malloc_failed(p->final_variant_id);
  p->variant_per_bin = malloc(num_devices * sizeof(uint8_t *));
  return_if_malloc_failed(p->variant_per_bin);
  //------------------------------------------
  for ( int i = 0; i < num_devices; i++ ) { 
    p->variant_per_bin[i] = malloc(AB_NUM_BINS * sizeof(uint8_t));
    return_if_malloc_failed(p->variant_per_bin[i]);
  }
BYE:
  return status;
}
