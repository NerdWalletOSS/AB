#include "ab_incs.h"
#include "ab_globals.h"
#include "del_test.h"

//----------------------------------------------------------
//<hdr>
// Called from Lua
int 
del_test(
    const char *args
    )
//</hdr>
{
  int status = 0;
  int32_t test_idx = 0;
  // test_idx = call_lua(args)
  if ( test_idx < 0 ) { go_BYE(-1); }
  zero_test(test_idx);
BYE:
  return status;
}
