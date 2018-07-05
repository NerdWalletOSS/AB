#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "l_mdl_meta.h"

int 
l_mdl_meta(
    void
    )
{
  int status = 0;
  if ( g_L_DT == NULL ) { go_BYE(-1); }
  lua_getglobal(g_L_DT, "get_mdl_meta");
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua Function get_mdl_meta undefined\n");
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  lua_pushlightuserdata(g_L_DT, g_rslt);
  lua_pushnumber(g_L_DT, AB_MAX_LEN_RESULT);
  status = lua_pcall(g_L_DT, 2, 0, 0);
  if ( status != 0 ) {
    fprintf(stderr, "calling function make_feature_vector failed: %s\n", lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
BYE:
  return status;
}
