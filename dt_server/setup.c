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
extern char g_dt_features[DT_MAX_NUM_FEATURES][DT_MAX_LEN_FEATURE+1]; 
extern int g_n_dt_features;

int
setup(
    const char *config_file
    )
{
  int status = 0;
  const char *model_dir = NULL;
  const char *model_name = NULL;
  const char *forest_type = NULL;
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
  status = get_forest_type(&forest_type); cBYE(status);
  status = load_models(model_dir, forest_type, num_features, g_interp); 
  //-- Set up features on C sise of the fence`
  g_n_dt_features = num_features;
  status = get_features(); 
  cBYE(status);
BYE:
  return status;
}
