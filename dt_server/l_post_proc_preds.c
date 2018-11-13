#include "dt_incs.h"
#include "auxil.h"
#include "dt_globals.h"
#include "l_post_proc_preds.h"

int 
l_post_proc_preds(
    const char *args,
    float *pred_vector, // this is g_predictions [n_pred_vector]
    int n_pred_vector
    )
{
  int status = 0;
  const char *lua_fn = "post_proc_preds";
  char buf[16];
  memset(buf, '\0', 16);
  if ( args != NULL ) {
    status = extract_name_value(args, "Testing=", '&', buf, 15); cBYE(status);
    if ( strcasecmp(buf, "true") == 0 ) { 
      for ( int i = 0; i < n_pred_vector; i++ ) {
        pred_vector[i] = 1.0 / ( 1 + n_pred_vector);
      }
    }
  }
  lua_getglobal(g_L_DT, lua_fn);
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua Function %s undefined\n", lua_fn);
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  lua_pushlightuserdata(g_L_DT, pred_vector);
  lua_pushnumber(g_L_DT, n_pred_vector);
  status = lua_pcall(g_L_DT, 2, 1, 0);
  if ( status != 0 ) {
    fprintf(stderr, "calling function %s failed: %s\n", 
        lua_fn, lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  if (!lua_isstring(g_L_DT, -1)) {
    fprintf(stderr, "%s: return must be a string\n", __func__); go_BYE(-1); 
  }
  strncpy(g_rslt, lua_tostring(g_L_DT, -1), DT_MAX_LEN_RESULT);
BYE:
  return status;
}
