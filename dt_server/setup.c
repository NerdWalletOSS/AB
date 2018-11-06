#include <lua.h>
#include "dt_incs.h"
#include "dt_httpd_types.h"
#include "init.h"
#include "init.h"
#include "load_models.h"
#include "setup.h"
extern char g_err[DT_ERR_MSG_LEN+1]; 
extern lua_State *g_L_DT; 

static int
get_mdl_loc(
    const char **ptr_dt_dir,
    const char **ptr_model_name
    )
{
  int status = 0;
  lua_getglobal(g_L_DT, "get_mdl_loc");
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua function get_mdl_loc missing\n");
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  status = lua_pcall(g_L_DT, 0, 2, 0);
  if (status != 0) {
    fprintf(stderr, "Lua function get_mdl_loc failed: %s\n", 
          lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  // get dt_dir
  if (!lua_isstring(g_L_DT, -2)) {
    fprintf(stderr, "get_dt_dir: return 1 must be a string"); go_BYE(-1); 
  }
  *ptr_dt_dir = lua_tostring(g_L_DT, -1);
  // get model name 
  if (!lua_isstring(g_L_DT, -1)) {
    fprintf(stderr, "get_dt_dir: return 2 must be a string"); go_BYE(-1); 
  }
  *ptr_model_name = lua_tostring(g_L_DT, -2);
BYE:
  return status;
}
int
setup(
    const char *config_file
    )
{
  int status = 0;
  const char *dt_dir = NULL;
  const char *model_name = NULL;

  free_globals(); 
  zero_globals();
  status = init_lua(); cBYE(status); 
  status = get_mdl_loc(&dt_dir, &model_name); cBYE(status);
  status = load_models(dt_dir, model_name); cBYE(status);
BYE:
  return status;
}
