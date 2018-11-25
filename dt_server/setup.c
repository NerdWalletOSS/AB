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
  const char *dt_dir = NULL;
  const char *model_name = NULL;
  char *buf = NULL; size_t bufsz = 0;

  free_globals(); 
  zero_globals();
  status = init_lua(config_file); cBYE(status); 
  status = get_mdl_loc(&dt_dir, &model_name); cBYE(status);
  bufsz = strlen(dt_dir) + strlen(model_name) + 8;
  buf = malloc(bufsz); return_if_malloc_failed(buf);
  sprintf(buf, "%s/%s", dt_dir, model_name);
  status = load_models(buf, &g_interp); cBYE(status);
BYE:
  free_if_non_null(buf);
  return status;
}
