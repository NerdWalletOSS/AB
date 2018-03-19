#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "test_info.h"

int
l_test_info(
    const char *args
    )
{
  int status = 0;
  lua_getglobal(g_L, "get_test_info");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function add does not exist in lua's global space\n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  lua_pushstring(g_L, args); // not pushing string as it causes a copy
  status = lua_pcall(g_L, 1, 1, 0);
  if (status != 0) {
    fprintf(stderr, "calling function add failed: %s\n", lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
  }
  char* test_info = (char *)lua_tostring(g_L, 1);
  if (AB_MAX_LEN_RESULT < strlen(test_info)) {
    sprintf(g_err, "{\"error\": Length of test info is too much for buffer (max=%" PRIu32", actual=%" PRIu64 ")",
        AB_MAX_LEN_RESULT, strlen(test_info));
    lua_pop(g_L, 1);
    go_BYE(-1);
  } else {
    memcpy(g_buf, test_info, strlen(test_info));
    lua_pop(g_L, 1);
  }

BYE:
  return status;
}
