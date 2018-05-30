#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "get_test_idx.h"
#include "l_list_tests.h"

int
l_list_tests(
    const char *args
    )
{
  int status = 0;
  int bufsz = 7;
  char buf[bufsz+1]; 
  status = extract_name_value(args, "Source=", '&', buf, bufsz);
  cBYE(status);

  if ( ( *buf == '\0' ) || ( strcmp(buf, "Lua") == 0 ) ) {
    if ( g_L == NULL ) { go_BYE(-1); }
    lua_getglobal(g_L, "list_tests");
    if ( !lua_isfunction(g_L, -1)) {
      fprintf(stderr, "Function add does not exist in lua's global space\n");
      lua_pop(g_L, 1);
      go_BYE(-1);
    }

    status = lua_pcall(g_L, 0, 1, 0);
    if (status != 0) {
      WHEREAMI;
      fprintf(stderr, "calling function list_tests failed: %s\n", lua_tostring(g_L, -1));
      sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
      lua_pop(g_L, 1);
    }

    char* tests = (char *)lua_tostring(g_L, 1);
    if (AB_MAX_LEN_RESULT < strlen(tests)) {
      sprintf(g_err, "{\"error\": Length of list tests is too much for buffer (max=%" PRIu32", actual=%" PRIu64 ")",
          AB_MAX_LEN_RESULT, strlen(tests));
      lua_pop(g_L, 1);
      go_BYE(-1);
    } else {
      memcpy(g_rslt, tests, strlen(tests));
      lua_pop(g_L, 1);
    }
  }
  else if ( ( *buf != '\0' ) && ( strcmp(buf, "C") == 0 ) ) {
    int test_type;
    status = get_test_type(args, &test_type); cBYE(status);
    status = c_list_tests(test_type, g_rslt, AB_MAX_LEN_RESULT); cBYE(status);
  }
  else {
    go_BYE(-1);
  }
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
    sprintf(buf, " %s ", g_tests[i].name); 
    strcat(X, buf);
  }
  strcat(X, " ]\n");
BYE:
  return status;
}

