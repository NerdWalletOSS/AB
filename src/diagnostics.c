#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "ab_constants.h"
#include "macros.h"
#include "ab_types.h"
#include "auxil.h"
#include "get_test_idx.h"
#include "diagnostics.h"

#include "ab_globals.h"

//<hdr>
int
diagnostics(
    void
    )
//</hdr>
{
  int status = 0;
  bool diff_found = false;
#ifdef XXX
  uint64_t xids[AB_MAX_NUM_TESTS];
  //--------------------------------------------------------
  //C \begin{invariant}
  //C uses\_eee can be 1 or 0 for ABTest, 0 for XYTest
  //C has\_filters  is false for XYTests
  //C is\_sequential  is false for XYTests
  //C is\_dev_specific  is false for ABTests
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) {
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    switch ( g_tests[i].test_type ) {
      case AB_TEST_TYPE_XY : 
        if ( g_tests[i].uses_eee != 0 ) { go_BYE(-1); }
        if ( g_tests[i].has_filters ) { go_BYE(-1); }
        if ( g_tests[i].is_sequential ) { go_BYE(-1); }
        break;
      case AB_TEST_TYPE_AB : 
        if ( g_tests[i].is_dev_specific ) { go_BYE(-1); }
        if ( ( ( g_tests[i].uses_eee != 0 ) && 
            ( g_tests[i].uses_eee != 1 ) ) || 
            ( ( g_tests[i].has_filters != 0 ) && 
              ( g_tests[i].has_filters != 1 ) ) || 
            ( ( g_tests[i].is_sequential != 0 ) && 
              ( g_tests[i].is_sequential != 1 ) )
            )
        { go_BYE(-1); }
        break;
      default : 
        go_BYE(-1);
        break;
    }
  }
  //C \end{invariant}
  //--------------------------------------------------------
  //C \begin{invariant}
  // Final variant must be one of variants for this test 
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) {
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    if ( g_tests[i].state != TEST_STATE_TERMINATED ) { continue; }
    //----------------------------------
    bool found = false;
    for ( int j = 0; j < g_tests[i].num_variants; j++ ) { 
      if ( g_tests[i].final_x_var_id == g_tests[i].x_var_id[j] ) {
        found = true; break;
      }
    }
    if ( !found ) { go_BYE(-1); }
    //----------------------------------
    found = false;
    for ( int j = 0; j < g_tests[i].num_variants; j++ ) { 
      if (strcmp(g_tests[i].final_variant, 
            g_tests[i].variants[j] ) == 0 ) {
        found = true; break;
      }
    }
    if ( !found ) { go_BYE(-1); }
    //----------------------------------
    found = false;
    if ( g_tests[i].final_var_url[0] != '\0' ) { 
    for ( int j = 0; j < g_tests[i].num_variants; j++ ) { 
      if (strcmp(g_tests[i].final_var_url, g_tests[i].var_url[j]) == 0 ) {
        found = true; break;
      }
    }
    if ( !found ) { go_BYE(-1); }
    }
  }

  //--------------------------------------------------------
  //C \begin{invariant}
  //C If test is in state ``started'', \verb+final_x_var_id+ = 0;
  //C If test is in state ``terminated'', \verb+final_x_var_id+ \(\neq 0\);
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    switch ( g_tests[i].state ) { 
      case TEST_STATE_TERMINATED : 
        if ( g_tests[i].final_x_var_id == 0 ) { go_BYE(-1); }
        if ( g_tests[i].final_variant[0] == '\0' ) { go_BYE(-1); }
        if ( g_tests[i].test_type == AB_TEST_TYPE_XY ) { 
          if ( g_tests[i].final_var_url[0] == '\0' ) { 
            fprintf(stderr, "Null description for final variant of test %s \n", g_tests[i].name);
            go_BYE(-1); 
          }
        }
        break;
      case TEST_STATE_STARTED : 
        if ( g_tests[i].final_x_var_id != 0 ) { go_BYE(-1); }
        for ( int j = 0; j < AB_MAX_LEN_VARIANT_NAME; j++ ) { 
          if ( g_tests[i].final_variant[j] != '\0' ) { go_BYE(-1); }
        }
        for ( int j = 0; j < AB_MAX_LEN_VARIANT_URL; j++ ) { 
          if ( g_tests[i].final_var_url[j] != '\0' ) { go_BYE(-1); }
        }
        break;
      default : 
        go_BYE(-1);
        break;
    }
  }
  //C \end{invariant}
  //----------------------------------------------
  //C \begin{invariant}
  //C \label{uq_test_name}
  //C Within a particular test type, Name of tests should be unique 
  for ( int k = 0; k < 2; k++ ) {
    int test_type = -1;
    switch ( k ) {
      case 0 : test_type = AB_TEST_TYPE_AB; break;
      case 1 : test_type = AB_TEST_TYPE_XY; break;
      default : go_BYE(-1); break;
    }
    for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
      if ( g_tests[i].name[0] == '\0' ) { continue; }
      if ( g_tests[i].test_type != test_type ) { continue; }
      for ( int j = i+1; j < AB_MAX_NUM_TESTS; j++ ) { 
        if ( g_tests[j].name[0] == '\0' ) { continue; }
        if ( g_tests[j].test_type != test_type ) { continue; }
        if ( strcmp(g_tests[i].name, g_tests[j].name) == 0 ) { go_BYE(-1); }
      }
    }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant} 
  //C Data for variants not in use should be null
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    for ( int j = 0; j < g_tests[i].num_variants; j++ ) { 
      if ( g_tests[i].x_var_id[j]    == 0 ) { go_BYE(-1); }
      if ( g_tests[i].variants[j][0] == '\0' ) { go_BYE(-1); }
      if ( g_tests[i].test_type == AB_TEST_TYPE_XY ) { 
        if ( g_tests[i].var_url[j][0]  == '\0' ) { 
          go_BYE(-1); }
      }
    }
    for ( int j = g_tests[i].num_variants; j < AB_MAX_NUM_VARIANTS; j++ ) { 
      if ( g_tests[i].x_var_id[j]    != 0 ) { go_BYE(-1); }
      for ( int k = 0; k < AB_MAX_LEN_VARIANT_NAME+1; k++ ) { 
        if ( g_tests[i].variants[j][k] != '\0' ) { 
          go_BYE(-1); }
      }
      for ( int k = 0; k < AB_MAX_LEN_VARIANT_URL+1; k++ ) { 
        if ( g_tests[i].var_url[j][k]  != '\0' ) { go_BYE(-1); }
      }
      if ( g_tests[i].counts[j]      != 0 ) { go_BYE(-1); }
      if ( g_tests[i].proportions[j] != 0 ) { go_BYE(-1); }
    }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant} 
  //C \verb+num_variants+ must be in \([x, y]\), includes Control 
  //C where \(x\) = AB\_MIN\_NUM\_VARIANTS
  //C and \(y\) = AB\_MAX\_NUM\_VARIANTS
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    if ( ( g_tests[i].num_variants < AB_MIN_NUM_VARIANTS ) || 
        ( g_tests[i].num_variants > AB_MAX_NUM_VARIANTS  ) ) {
      go_BYE(-1);
    }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant} 
  //C Spot in use \(\Rightarrow \) ramp \(\geq 1\)
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    if ( g_tests[i].ramp_num < 1 ) { go_BYE(-1); }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant} 
  //C \verb+variants+ and \verb+x_var_id+ within test must be unique
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    for ( int j = 0; j < g_tests[i].num_variants; j++ ) { 
      for ( int k = j+1; k < g_tests[i].num_variants; k++ ) { 
        if ( strcmp(g_tests[i].variants[j], g_tests[i].variants[k]) == 0 ) {
          go_BYE(-1);
        }
        if ( g_tests[i].test_type == AB_TEST_TYPE_XY ) { 
          if ( strcmp(g_tests[i].var_url[j], g_tests[i].var_url[k]) == 0 ) {
            go_BYE(-1);
          }
        }
        if ( g_tests[i].x_var_id[j] == g_tests[i].x_var_id[k] )  {
          go_BYE(-1);
        }
      }
    }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant} 
  //C \verb+x_tst_id+ and \verb+x_var_id+ must be unique across all
  int tempbuf[AB_MAX_NUM_TESTS*AB_MAX_NUM_VARIANTS];
  int tempbufcnt = 0;
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    tempbuf[tempbufcnt++] = g_tests[i].x_tst_id;
  }
  for ( int i = 0; i < tempbufcnt; i++ ) { 
    for ( int j = i+1; j < tempbufcnt; j++ ) { 
      if ( tempbuf[i] == tempbuf[j] ) { go_BYE(-1); }
    }
  }
  tempbufcnt = 0;
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    for ( int j = 0; j < g_tests[i].num_variants; j++ ) { 
      tempbuf[tempbufcnt++] = g_tests[i].x_var_id[j];
    }
  }
  for ( int i = 0; i < tempbufcnt; i++ ) { 
    for ( int j = i+1; j < tempbufcnt; j++ ) { 
      if ( tempbuf[i] == tempbuf[j] ) { go_BYE(-1); }
    }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant} 
  //C State of test must be one of \{started, terminated\}
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    if ( ( g_tests[i].state != TEST_STATE_STARTED ) &&
        ( g_tests[i].state != TEST_STATE_TERMINATED ) ) { 
      go_BYE(-1);
    }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant} 
  //C Every element of \verb+variant_per_bin+ must have a value in 
  //C \([0, num\_variants-1]\) 
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    for ( int j = 0; j < AB_NUM_BINS; j++ ) { 
      if ( g_tests[i].variant_per_bin[j] >= g_tests[i].num_variants ) {
        go_BYE(-1);
      }
    }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant} 
  //C \(\sum_i\) \verb+variant_per_bin[i]+ ) proportions[i] \(\times \) 10 
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    status = chk_variant_per_bin( g_tests[i].proportions,
        g_tests[i].num_variants, g_tests[i].variant_per_bin);
    cBYE(status);
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant}
  //C Name is null \(\Rightarrow\) all other fields are 0 or null
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] != '\0' ) { continue; }
    char *cptr = (char *)(&(g_tests[i]));
    size_t sz = sizeof(TEST_META_TYPE);
    for ( size_t j = 0; j < sz; j++, cptr++ ) { 
      if ( *cptr != 0 ) { go_BYE(-1); }
    }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant} 
  //C Counts and proportions must be roughly in sync
#undef CHECK_COUNTS_IN_RTS
#ifdef CHECK_COUNTS_IN_RTS
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) {
  float observed_proportion[AB_MAX_NUM_VARIANTS];
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    float sum = 0;
    int allowable_diff = 16;
    for ( int j = 0; j < AB_MAX_NUM_VARIANTS; j++ ) {
      sum += g_tests[i].counts[j];
    }
    if ( sum < 128 ) { continue ; } // Not enough data 
    for ( int x = 128; x <= sum; x *= 2 ) {
      if ( allowable_diff == 2 ) { break; }
      allowable_diff--;
    }
    for ( int j = 0; j < AB_MAX_NUM_VARIANTS; j++ ) {
      observed_proportion[j] = 100.0 * (float)g_tests[i].counts[j] / sum;
    }
    bool diff = false;
    for ( int j = 0; j < AB_MAX_NUM_VARIANTS; j++ ) {
      float delta = abs(observed_proportion[j] - g_tests[i].proportions[j]);
      if ( delta > allowable_diff ) { 
        diff = true;
        break;
      }
    }
    if ( diff ) { 
      diff_found = true;
      fprintf(stderr, "Error in counts for test %d:%s \n", 
          g_tests[i].test_type, g_tests[i].name);
      fprintf(stderr, "tst_id,var_id,count,proportion\n");
      for ( int j = 0; j < g_tests[i].num_variants; j++ ) {
        fprintf(stderr, "%d,%d,%d,%d\n",
            g_tests[i].x_tst_id, g_tests[i].x_var_id[j], 
            g_tests[i].counts[j], g_tests[i].proportions[j]);
      }
      fprintf(stderr, "-----------------------------\n");
    }
  }
#endif
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant}
  //C Count must be 0 if proportion is 0 
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    for ( int j = 0; j < AB_MAX_NUM_VARIANTS; j++ ) {
      if ( g_tests[i].proportions[j] == 0 ) {
        if ( g_tests[i].counts[j] != 0 ) { 
          go_BYE(-1); 
        }
      }
    }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant}
  //C Proportions must add up to 100.
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    int sum =  0;
    for ( int j = 0; j < g_tests[i].num_variants; j++ ) { 
      if ( g_tests[i].proportions[j] > 100 ) { go_BYE(-1); }
      sum += g_tests[i].proportions[j];
    }
    if ( sum != 100 ) { go_BYE(-1); }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant}
  //C An XYTest must have non-null, unique URL for each variant.
  //C An ABTest must have null URLs for each variant.
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    switch ( g_tests[i].test_type ) {
      case AB_TEST_TYPE_AB : 
        for ( int j = 0; j < AB_MAX_NUM_VARIANTS; j++ ) { 
          for ( int k = 0; k < AB_MAX_LEN_VARIANT_URL+1; k++ ) { 
            if ( g_tests[i].var_url[j][k] != '\0' ) {  go_BYE(-1); }
          }
        }
        break; 
      case AB_TEST_TYPE_XY : 
        for ( int j = 0; j < g_tests[i].num_variants; j++ ) { 
          if ( (g_tests[i].var_url[j])[0] == '\0' ) { 
            fprintf(stderr, "Error on test %d = %s \n", i, g_tests[i].name); 
            go_BYE(-1); }
        }
        for ( int j = 0; j < g_tests[i].num_variants; j++ ) { 
          for ( int k = j+1; k < g_tests[i].num_variants; k++ ) { 
            if ( strcmp(g_tests[i].var_url[j], g_tests[i].var_url[k]) == 0 ) {
              go_BYE(-1);
            }
          }
        }
        break;
      default : 
        go_BYE(-1);
        break;
    }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant}
  //C \verb+external_id+ must be non-zero for tests  of type XY
  //C \verb+external_id+ must be     zero for tests  of type AB
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    switch ( g_tests[i].test_type ) { 
      case AB_TEST_TYPE_XY : 
        if ( g_tests[i].external_id == 0 ) { 
          go_BYE(-1); 
        }
        break;
      case AB_TEST_TYPE_AB : 
        if ( g_tests[i].external_id != 0 ) { go_BYE(-1); }
        break;
      default : 
        go_BYE(-1);
        break;
    }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant}
  //C \verb+external_id+ must be unique for tests of type XY
  int xidx = 0;
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    if ( g_tests[i].test_type != AB_TEST_TYPE_XY ) { continue; }
    xids[xidx++] = g_tests[i].external_id;
  }
  for ( int i = 0; i < xidx; i++ ) { 
    for ( int j = i+1; j < xidx; j++ ) { 
      if ( xids[i] == xids[j] ) { 
        go_BYE(-1);
      }
    }
  }
  //C \end{invariant}
  //-----------------------------------------------
  //C \begin{invariant}
  //C Only AB testst can have is\_sequential set. 
  //C For those that do, chk\_is\_sequential must pass 
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] == '\0' ) { continue; }
    if ( g_tests[i].test_type == AB_TEST_TYPE_AB ) { 
      if ( g_tests[i].is_sequential  ) { 
        if ( !chk_is_sequential(g_tests[i].proportions, 
              g_tests[i].num_variants) ) {
          go_BYE(-1);
        }
      }
    }
    else {
      if ( g_tests[i].is_sequential  ) { 
        go_BYE(-1);
      }
    }
  }
  //C \end{invariant}
  //-----------------------------------------------
#endif
BYE:
  if ( ( diff_found ) && ( status == 0 ) ) {
    status = 1;
  }
  return status;
}
