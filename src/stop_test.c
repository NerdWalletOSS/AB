#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "get_test_idx.h"
#include "aux_zero.h"
#include "stop_test.h"

int
stop_test(
    const char *args
    )
{
  int status = 0;
  int test_idx, test_type;

  status = get_test_from_args(args, &test_idx, &test_type); cBYE(status);
  srandom(RDTSC());
  int nV = g_tests[test_idx].num_variants;
  int winner_idx = random() % nV;
  int winner_id  = g_tests[test_idx].variants[winner_idx].id;
  int num_devices = 1;
  if ( g_tests[test_idx].is_dev_specific ) { 
    num_devices = g_n_justin_cat_lkp;
  }
  for ( int d = 0; d < num_devices; d++ ) { 
    g_tests[test_idx].final_variant_idx[d] = winner_idx;
    g_tests[test_idx].final_variant_id[d]  = winner_id ;
    free_if_non_null(g_tests[test_idx].variant_per_bin[d]);
  }
  free_if_non_null(g_tests[test_idx].variant_per_bin);
BYE:
  return status;
}
