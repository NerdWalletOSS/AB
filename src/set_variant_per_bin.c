#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "ab_constants.h"
#include "ab_types.h"
#include "ab_globals.h"
#include "macros.h"
#include "ab_types.h"
#include "auxil.h"
#include "set_variant_per_bin.h"

int 
set_variant_per_bin(
    uint32_t *proportions,
    int num_variants, /* this includes control */
    uint8_t *variant_per_bin
    )
{
  int status = 0;
  // start by setting everybody to Control
  for ( int i = 0; i < AB_NUM_BINS; i++ ) { variant_per_bin[i] = 0; }
  // First, for each variant, use dedicated bins 
  // Dedicated bins for variant i are i, i+(1*nV), i+(2*nV), ...
  for ( int v = 1; v < num_variants; v++ ) { 
    if ( proportions[v] == 0 ) { continue; }
    int bins_to_set = 
      (int)((double)proportions[v] / 100.0 * AB_NUM_BINS );
    int chk_bins_to_set = 0;
    for ( int idx = v; idx < AB_NUM_BINS; idx += num_variants ) {
      if ( chk_bins_to_set == bins_to_set ) { break; }
      chk_bins_to_set++;
      variant_per_bin[idx] = v;
    }
    // If you don't have enough of your bins, steal from Control
    // Note that variant i can steal bin j only if ((j/nV) % (i-1)) == 0
    // So, if nV = 3, 
    // Control's bins are 0, 3, 6, 9, 12, 15, ... These are renumbered
    // by dividing by 3:  0, 1, 2, 3, 4,  5,  
    // Variant 1 gets renumbered bins 0, 2, 4, ....
    // Variant 1 gets original   bins 0, 6, 12, ....
    // Variant 2 gets renumbered bins 1, 3, 5, ...
    // Variant 2 gets original   bins 3, 9, 15, ...
    if ( chk_bins_to_set > bins_to_set ) { go_BYE(-1); }
    int start = (v-1)*num_variants;
    int incr  = (num_variants-1)*num_variants;
    for ( int idx = start; idx < AB_NUM_BINS; idx += incr ) { 
      if ( chk_bins_to_set == bins_to_set ) { break; }
      variant_per_bin[idx] = v;
      chk_bins_to_set++;
    }
    if ( chk_bins_to_set != bins_to_set ) { go_BYE(-1); }
  }
BYE:
  return status;
}

int 
alt_set_variant_per_bin(
    uint32_t *proportions,
    int num_variants, /* this includes control */
    uint8_t *variant_per_bin
    )
{
  int status = 0;
  int bins_to_set[AB_MAX_NUM_VARIANTS];
  int bins_set[AB_MAX_NUM_VARIANTS];

  if ( ( num_variants < AB_MIN_NUM_VARIANTS ) || 
      ( num_variants > AB_MAX_NUM_VARIANTS ) ) {
    go_BYE(-1);
  }
  if ( proportions == NULL ) { go_BYE(-1); }
  if ( variant_per_bin == NULL ) { go_BYE(-1); }
  for ( int i = 0; i < AB_NUM_BINS; i++ ) { variant_per_bin[i] = 0; }
  for ( int i = 0; i < AB_MAX_NUM_VARIANTS; i++ ) { bins_to_set[i] = 0; }
  for ( int i = 0; i < AB_MAX_NUM_VARIANTS; i++ ) { bins_set[i] = 0; }

  int sum_bins_to_set = 0;
  int actual_bins_set = 0;
  for ( int v = 0; v < num_variants; v++ ) { 
    if ( proportions[v] == 0 ) { continue; }
    bins_to_set[v] = 
      (int)((double)proportions[v] / 100.0 * AB_NUM_BINS );
    sum_bins_to_set += bins_to_set[v];
  }
  //-------------------------------------------------
  int idx = 0;
  for ( ; actual_bins_set < sum_bins_to_set; ) {
    for ( int v = 0; v < num_variants; v++ ) { 
      if ( proportions[v] == 0 ) { continue; }
      if ( bins_set[v] < bins_to_set[v] ) { 
        variant_per_bin[idx++] = v;
        bins_set[v]++;
        actual_bins_set++;
      }
    }
  }
  //-- Cover balance if any 
  for ( ; idx < AB_NUM_BINS; idx++ ) { 
    for ( int v = 0; v < num_variants; v++ ) { 
      if ( proportions[v] == 0 ) { continue; }
      variant_per_bin[idx++] = v;
      if ( idx >= AB_NUM_BINS ) { break; }
    }
  }
BYE:
  return status;
}
int
chk_variant_per_bin(
    uint32_t *proportions,
    uint32_t num_variants,
    uint8_t *variant_per_bin
    )
{
  int status = 0;
  float chk_proportions[AB_MAX_NUM_VARIANTS];
  for ( int j = 0; j <  AB_MAX_NUM_VARIANTS; j++ ) { 
    chk_proportions[j] = 0;
  }
  for ( int j = 0; j < AB_NUM_BINS; j++ ) { 
    uint32_t v = variant_per_bin[j];
    if ( v >= num_variants ) { go_BYE(-1); }
    chk_proportions[v]++;
  }
  for ( int j = 0; j < AB_MAX_NUM_VARIANTS; j++ ) { 
    chk_proportions[j] = (chk_proportions[j]*100.0/AB_NUM_BINS);
    if ( ( fabs(chk_proportions[j] - proportions[j] ) ) > 1 ) {
      go_BYE(-1);
    }
  }
BYE:
  return status;
}
#ifdef STAND_ALONE
int
main()
{
  int status = 0;
  uint32_t proportions[4];
  uint8_t variant_per_bin[AB_NUM_BINS];
  proportions[0] = 7;
  proportions[1] = 18;
  proportions[2] = 34;
  proportions[3] = 41;
  int num_variants = 4;
  status = set_variant_per_bin( proportions, num_variants, 
      variant_per_bin);
  cBYE(status);
BYE:
  return status;
}
#endif
  
