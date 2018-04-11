#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "l_post_proc_preds.h"

int 
l_post_proc_preds(
    float *pred_vector, // TODO P3 give better name 
    int n_pred_vector,
    char *X,
    size_t nX
    )
{
  int status = 0;
  lua_getglobal(g_L_DT, "post_proc_preds");
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua Function post_proc_preds undefined\n");
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  lua_pushlightuserdata(g_L_DT, pred_vector);
  lua_pushnumber(g_L_DT, n_pred_vector);
  lua_pushlightuserdata(g_L_DT, X);
  lua_pushnumber(g_L_DT, nX);
  status = lua_pcall(g_L_DT, 4, 0, 0);
  if ( status != 0 ) {
    fprintf(stderr, "calling function post_proc_preds failed: %s\n", lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
BYE:
  return status;
}
