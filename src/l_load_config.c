#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "l_load_config.h"
#include "update_config.h"


int
l_load_config(
    const char *file_name
    )
{
  int status = 0;
  if ( g_disable_lua ) { return 0; }
  if ( g_L == NULL ) { go_BYE(-1); }
  lua_getglobal(g_L, "load_config");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function load_config does not exist in lua's global space\n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  lua_pushstring(g_L, file_name); // not pushing string as it causes a copy
  status = lua_pcall(g_L, 1, 0, 0);
  if (status != 0) {
    WHEREAMI;
    fprintf(stderr, "calling function load_config failed: %s\n", lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  // Now to load ML lua transforms
  if ( g_L_DT != NULL ) {
    lua_getglobal(g_L_DT, "load_config");
    if ( !lua_isfunction(g_L_DT, -1)) {
      fprintf(stderr, "Function load_config does not exist in DT lua's global space\n");
      lua_pop(g_L_DT, 1);
      go_BYE(-1);
    }
    lua_pushstring(g_L_DT, file_name); // not pushing string as it causes a copy
    status = lua_pcall(g_L_DT, 1, 0, 0);
    if (status != 0) {
      WHEREAMI;
      fprintf(stderr, "calling function load_config for DT failed: %s\n", lua_tostring(g_L_DT, -1));
      sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
      lua_pop(g_L_DT, 1);
      go_BYE(-1);
    }
  }

BYE:
  return status;
}



