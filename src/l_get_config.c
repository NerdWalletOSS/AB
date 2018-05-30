#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "l_get_config.h"

int
l_get_config(
    void
    )
{
  int status = 0;
  lua_getglobal(g_L, "get_config");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function add does not exist in lua's global space\n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }

  status = lua_pcall(g_L, 0, 1, 0);
  if (status != 0) {
    WHEREAMI;
    fprintf(stderr, "calling function get_config failed: %s\n", lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
  }

  char* tests = (char *)lua_tostring(g_L, 1);
  if (AB_MAX_LEN_RESULT < strlen(tests)) {
    sprintf(g_err, "{\"error\": Length of configs is too much for buffer (max=%" PRIu32", actual=%" PRIu64 ")",
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