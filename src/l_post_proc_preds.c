#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "l_post_proc_preds.h"

int 
l_post_proc_preds(
    const char *args,
    float *pred_vector, // this is g_predictions [n_pred_vector]
    int n_pred_vector,
    char *X,
    size_t nX
    )
{
  int status = 0;
  if ( g_L_DT == NULL ) { go_BYE(-1); }
  char buf[16];
  memset(buf, '\0', 16);
  if ( args != NULL ) { 
  status = extract_name_value(args, "Testing=", "&", buf, 15); cBYE(status);
  if ( strcasecmp(buf, "true") == 0 ) { 
    for ( int i = 0; i < n_pred_vector; i++ ) {
      pred_vector[i] = 1.0 / ( 1 + n_pred_vector);
    }
  }
  }
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
