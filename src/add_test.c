#include "ab_incs.h"
#include "auxil.h"
#include "set_variant_per_bin.h"
#include "add_test.h"
#include "zero.h"
#include "ab_globals.h"

int 
add_test(
    const char *args
    )
{
  int status = 0;
  int test_idx = -1;
  // TODO test_idx =  lua_ add_test(args); 
  if ( test_idx < 0 ) { go_BYE(-1); }
BYE:
  return status;
}
