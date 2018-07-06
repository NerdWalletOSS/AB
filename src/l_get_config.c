#include "ab_incs.h"
#include "auxil.h"
#include "l_get_config.h"

#include <lua.h>
extern char g_rslt[AB_MAX_LEN_RESULT+1]; // For C: ab_process_req()
extern char g_err[AB_MAX_LEN_RESULT+1]; // For C: ab_process_req()
extern lua_State *g_L;

int
l_get_config(
    void
    )
{
  int status = 0;
  if ( g_L == NULL ) { go_BYE(-1); }
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

  char* config = (char *)lua_tostring(g_L, 1);
  if (AB_MAX_LEN_RESULT < strlen(config)) {
    sprintf(g_err, "{\"error\": Length of configs is too much for buffer (max=%" PRIu32", actual=%" PRIu64 ")",
        AB_MAX_LEN_RESULT, strlen(config));
    lua_pop(g_L, 1);
    go_BYE(-1);
  } 
  else {
    memcpy(g_rslt, config, strlen(config));
    lua_pop(g_L, 1);
  }

BYE:
  return status;
}
