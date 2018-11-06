#include <lua.h>
#include "dt_incs.h"
#include "dt_httpd_types.h"
#include "init.h"
#include "init.h"
#include "load_models.h"
#include "setup.h"
#include "get_from_lua.h"
extern char g_err[DT_ERR_MSG_LEN+1]; 
extern lua_State *g_L_DT; 

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
