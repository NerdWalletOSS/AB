#include "dt_incs.h"
#include "dt_httpd_types.h"
#include <lua.h>
#include "auxil.h"
#include "l_get_num_features.h"
extern lua_State *g_L_DT; 
extern  CFG_TYPE g_cfg; 
extern bool g_disable_lua;
extern char g_err[DT_ERR_MSG_LEN+1]; 

extern CFG_TYPE g_cfg;
// Ideally returns the number of features.
// If screws up, returns -1
int
l_get_num_features(
    int *ptr_num_features
    )
{
  int status = 0;
  if ( g_disable_lua ) { 
    *ptr_num_features = 1; return status; 
  }
  if ( *g_cfg.dt_dir == '\0' ) { go_BYE(-1); }
  *ptr_num_features = 0;

  lua_getglobal(g_L_DT, "get_num_features");
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Function get_num_features does not exist\n");
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  status = lua_pcall(g_L_DT, 0, 1, 0);
  if (status != 0) {
    fprintf(stderr, "get_num_features failed: %s\n", 
        lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  int num_features = lua_tonumber(g_L_DT, -1);
  lua_pop(g_L_DT, 1);
  if ( num_features <= 0 ) { go_BYE(-1); }
  *ptr_num_features = num_features;
BYE:
  return status;
}
