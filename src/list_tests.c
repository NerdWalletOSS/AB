#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "get_test_idx.h"
#include "list_tests.h"

int
list_tests(
    const char *args
    )
{
  int status = 0;
  int test_type;
  status = get_test_type(args, &test_type); cBYE(status);
  status = c_list_tests(test_type, g_rslt, AB_MAX_LEN_RESULT); cBYE(status);
BYE:
  return status;
}

int
c_list_tests(
    int test_type,
    char *X,
    size_t nX
    )
{
  int status = 0;
  bool first = true;
  char buf[2*(AB_MAX_LEN_TEST_NAME+1)];
  if ( ( X == NULL ) || ( nX <= 1024 ) ) { go_BYE(-1); }
  strcpy(X, " [ ");
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) { 
    if ( g_tests[i].name_hash == 0 ) { continue; }
    if ( g_tests[i].test_type != test_type ) { continue; }
    memset(buf,'\0', 2*(AB_MAX_LEN_TEST_NAME+1));
    if ( first ) { 
      first = false;
    }
    else {
      strcat(X, ", ");
    }
    sprintf(buf, " \"%s\" ", g_tests[i].name); 
    strcat(X, buf);
  }
  strcat(X, " ]\n");
BYE:
  return status;
}

