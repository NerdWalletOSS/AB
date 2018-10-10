#include "dt_incs.h"
#include "auxil.h"
#include "dt_globals.h"
#include "l_update_config.h"

int 
l_update_config(
    void
    )
{
  int status = 0;
  if ( g_disable_lua ) { return status; }
  if ( g_L_DT == NULL ) { go_BYE(-1); }
  lua_getglobal(g_L_DT, "update_config");
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "update_config() does not exist\n");
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  lua_pushlightuserdata(g_L_DT, &g_cfg);
  status = lua_pcall(g_L_DT, 1, 0, 0);
  if ( status != 0 ) {
    fprintf(stderr, "calling update_config() failed: %s\n", 
        lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
BYE:
  return status;
}
