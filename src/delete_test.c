#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "get_test_idx.h"
#include "aux_zero.h"
#include "delete_test.h"

int
delete_test(
    const char *args
    )
{
  int status = 0;
  int test_type, test_idx;

  status = get_test_from_args(args, &test_idx, &test_type); cBYE(status);
  status = zero_test(test_idx); go_BYE(-1);
BYE:
  return status;
}
