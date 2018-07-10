#include "ab_incs.h"
#include "num_tests.h"

extern  TEST_META_TYPE g_tests[AB_MAX_NUM_TESTS]; 

int
num_tests(
    int *ptr_num_tests
    )
{
  int status = 0;
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name[0] != '\0' ) { 
      *ptr_num_tests++;
    }
  }
BYE:
  return status;
}
