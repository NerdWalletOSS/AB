#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "list_tests.h"

int
l_list_tests(
    const char *args
    )
{
  int status = 0;
  lua_getglobal(g_L, "list_tests");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function add does not exist in lua's global space\n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  char* tests = (char *)lua_tostring(g_L, 1);
  if (AB_MAX_LEN_RESULT < strlen(tests)) {
    sprintf(g_err, "{\"error\": Length of list tests is too much for buffer (max=%" PRIu32", actual=%" PRIu64 ")",
        AB_MAX_LEN_RESULT, strlen(tests));
    lua_pop(g_L, 1);
    go_BYE(-1);
  } else {
    memcpy(g_buf, tests, strlen(tests));
    lua_pop(g_L, 1);
  }
BYE:
  return status;
}
