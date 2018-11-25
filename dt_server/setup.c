#include <lua.h>
#include "dt_incs.h"
#include "dt_httpd_types.h"
#include "init.h"
#include "init.h"
#include "load_models.h"
#include "setup.h"
#include "get_from_lua.h"
extern lua_State *g_L_DT; 
extern DT_INTERPRETER_TYPE *g_interp;

int
setup(
    const char *config_file
    )
{
  int status = 0;
  const char *model_dir = NULL;
  const char *model_name = NULL;
  int num_features;

  free_globals(); 
  zero_globals();
  //-- allocate C structure that stores decision trees
  g_interp = malloc(1 * sizeof(DT_INTERPRETER_TYPE));
  return_if_malloc_failed(g_interp);
  memset(g_interp, '\0',  1 * sizeof(DT_INTERPRETER_TYPE));
  //-----------------------------
  status = init_lua(config_file); cBYE(status); 
  status = get_mdl_loc(&model_dir, &model_name); cBYE(status);
  status = get_num_features(&num_features); cBYE(status);
  status = load_models(model_dir, num_features, g_interp); cBYE(status);
BYE:
  return status;
}
