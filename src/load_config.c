#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "load_config.h"

int 
l_load_config(
    const char *file_name
    )
{
  int status = 0;
  static char has_changed[NUM_SERVICES];
  memset(has_changed, 0, NUM_SERVICES);
  lua_getglobal(g_L, "load_config");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function load_config does not exist in lua's global space\n");
  lua_pop(g_L, 1);
    go_BYE(-1);
  }
  lua_pushlightuserdata(g_L, &g_cfg);
  lua_pushlightuserdata(g_L, has_changed);
  lua_pushstring(g_L, file_name); // not pushing string as it causes a copy
  status = lua_pcall(g_L, 3, 0, 0);
  if (status != 0) {
    fprintf(stderr, "calling function add failed: %s\n", lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
BYE:
  return status;
}
