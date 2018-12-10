#include "dt_incs.h"
#include <lua.h>
#include "auxil.h"
#include "l_mdl_meta.h"

extern char g_rslt[DT_MAX_LEN_RESULT+1]; // For C: ab_process_req()
extern char g_err[DT_ERR_MSG_LEN+1]; // For C: ab_process_req()
extern DT_INTERPRETER_TYPE *g_interp;
extern lua_State *g_L_DT;
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
  lua_pushnumber(g_L_DT, DT_MAX_LEN_RESULT);
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
