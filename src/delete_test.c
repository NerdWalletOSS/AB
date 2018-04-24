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
  int bufsz = 511; int test_type, idx;
  char test_name[AB_MAX_LEN_TEST_NAME+1]; 
  char *buf = NULL;

  buf = malloc(bufsz+1);
  memset(buf, '\0',  bufsz+1);
  //-----------------------------------------------
  memset(buf, '\0', bufsz+1);
  status = extract_name_value(args, "TestType=", '&', buf, bufsz);
  cBYE(status);
  if ( buf[0] == '\0' ) { go_BYE(-1); }
  if ( strcmp(buf, "ABTest") == 0 ) { 
    test_type = AB_TEST_TYPE_AB;
  }
  else if ( strcmp(buf, "XYTest") == 0 ) { 
    test_type = AB_TEST_TYPE_XY;
  }
  else {
    go_BYE(-1);
  }
  //-----------------------------------------------
  memset(test_name, '\0', AB_MAX_LEN_TEST_NAME+1);
  status = extract_name_value(args, "TestName=", '&', test_name, 
      AB_MAX_LEN_TEST_NAME);
  cBYE(status);
  if ( test_name[0] == '\0' ) { go_BYE(-1); }
  //-----------------------------------------------
  //-- Test must exist
  status = get_test_idx(test_name, test_type, &idx); 
  if ( ( status != 0 ) || ( idx < 0 ) )  { go_BYE(-1); }
  status = zero_test(idx); go_BYE(-1);
BYE:
  free_if_non_null(buf);
  return status;
}
