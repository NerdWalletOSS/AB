#include "dt_incs.h"
#include "auxil.h"
#include "dt_globals.h"
#include "l_make_feature_vector.h"

int 
l_make_feature_vector(
    const char * const fvec_as_json
    )
{
  int status = 0;
  const char *const lua_fn = "make_feature_vector";
  lua_getglobal(g_L_DT, lua_fn);
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua Function %s undefined\n", lua_fn);
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  lua_pushstring(g_L_DT, fvec_as_json); 
  lua_pushlightuserdata(g_L_DT, g_dt_feature_vector);
  lua_pushnumber(g_L_DT, g_n_dt_feature_vector);
  status = lua_pcall(g_L_DT, 3, 1, 0);
  if ( status != 0 ) {
    fprintf(stderr, "calling function %s failed: %s\n", 
        lua_fn, lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  if (!lua_isboolean(g_L_DT, -1)) {
    fprintf(stderr, "%s: return 1 must be a boolean\n", __func__); go_BYE(-1); 
  }
  bool l_status = lua_toboolean(g_L_DT, -1);
  if ( l_status != true ) { go_BYE(-1); }
BYE:
  return status;
}
