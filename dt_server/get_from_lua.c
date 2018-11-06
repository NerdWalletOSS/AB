#include <lua.h>
#include "dt_incs.h"
#include "dt_httpd_types.h"
#include "get_from_lua.h"
extern char g_err[DT_ERR_MSG_LEN+1]; 
extern lua_State *g_L_DT; 

int
get_mdl_loc(
    const char **ptr_dt_dir,
    const char **ptr_model_name
    )
{
  int status = 0;
  const char *const lua_fn = "get_mdl_loc";
  lua_getglobal(g_L_DT, lua_fn);
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua function %s missing\n", lua_fn);
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  status = lua_pcall(g_L_DT, 0, 2, 0);
  if (status != 0) {
    fprintf(stderr, "Lua function %s failed: %s\n", lua_fn,
          lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  // get dt_dir
  if (!lua_isstring(g_L_DT, -2)) {
    fprintf(stderr, "%s: return 1 must be a string\n", __func__); go_BYE(-1); 
  }
  *ptr_dt_dir = lua_tostring(g_L_DT, -2);
  // get model name 
  if (!lua_isstring(g_L_DT, -1)) {
    fprintf(stderr, "%s: return 2 must be a string\n", __func__); go_BYE(-1); 
  }
  *ptr_model_name = lua_tostring(g_L_DT, -1);
BYE:
  return status;
}

int
get_port(
    int *ptr_port
    )
{
  int status = 0;
  const char *const lua_fn = "get_port";
  lua_getglobal(g_L_DT, lua_fn);
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua function %s missing\n", lua_fn);
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  status = lua_pcall(g_L_DT, 0, 1, 0);
  if (status != 0) {
    fprintf(stderr, "Lua function %s failed: %s\n", lua_fn,
          lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  if (!lua_isnumber(g_L_DT, -1)) {
    fprintf(stderr, "%s: return 1 must be a number\n", __func__); go_BYE(-1); 
  }
  *ptr_port = lua_tonumber(g_L_DT, -1);
BYE:
  return status;
}

int
get_num_features(
    int *ptr_num_features
    )
{
  int status = 0;
  const char *const lua_fn = "get_num_features";
  lua_getglobal(g_L_DT, lua_fn);
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua function %s missing\n", lua_fn);
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  status = lua_pcall(g_L_DT, 0, 1, 0);
  if (status != 0) {
    fprintf(stderr, "Lua function %s failed: %s\n", lua_fn,
          lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  if (!lua_isnumber(g_L_DT, -1)) {
    fprintf(stderr, "%s: return 1 must be a number\n", __func__); go_BYE(-1); 
  }
  *ptr_num_features = lua_tonumber(g_L_DT, -1);
BYE:
  return status;
}

int
get_mdl_meta(
    const char **ptr_mdl_meta
    )
{
  int status = 0;
  const char *const lua_fn = "get_mdl_meta";
  lua_getglobal(g_L_DT, lua_fn);
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua function %s missing\n", lua_fn);
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  status = lua_pcall(g_L_DT, 0, 1, 0);
  if (status != 0) {
    fprintf(stderr, "Lua function %s failed: %s\n", lua_fn,
          lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  // get mdl_meta
  if (!lua_isstring(g_L_DT, -1)) {
    fprintf(stderr, "%s: return 1 must be a string\n", __func__); go_BYE(-1); 
  }
  *ptr_mdl_meta = lua_tostring(g_L_DT, -1);
BYE:
  return status;
}

int
get_config(
    const char **ptr_config
    )
{
  int status = 0;
  const char * const lua_fn = "get_config";
  lua_getglobal(g_L_DT, lua_fn);
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua function %s missing\n", lua_fn);
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  status = lua_pcall(g_L_DT, 0, 1, 0);
  if (status != 0) {
    fprintf(stderr, "Lua function %s failed: %s\n",  lua_fn,
          lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  // get config
  if (!lua_isstring(g_L_DT, -1)) {
    fprintf(stderr, "%s: return 1 must be a string\n", __func__); go_BYE(-1); 
  }
  *ptr_config = lua_tostring(g_L_DT, -1);
BYE:
  return status;
}

int
set_model_name(
    const char * model_name
    )
{
  int status = 0;
  mcr_chk_non_null(model_name);
  //----------------------------------------
  const char *const lua_fn = "set_model_name";
  lua_getglobal(g_L_DT, lua_fn);
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua function %s missing\n", lua_fn);
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  
  lua_pushstring(g_L_DT, model_name);
  status = lua_pcall(g_L_DT, 1, 0, 0);
  if (status != 0) {
    fprintf(stderr, "Lua function %s failed: %s\n", lua_fn, 
        lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  if (!lua_isstring(g_L_DT, -1)) {
    fprintf(stderr, "%s: return 1 must be a string\n", __func__); go_BYE(-1); 
  }
BYE:
  return status;
}
