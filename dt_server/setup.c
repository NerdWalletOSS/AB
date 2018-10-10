#include "dt_incs.h"
#include "zero_globals.h"
#include "init.h"
#include "update_config.h"
#include "read_conf_file.h"
#include "validate_config.h"
#include "setup.h"

int
setup(
    const char *config_file
    )
{
  int status = 0;

  free_globals(); 
  zero_globals();
  init_globals();
  status = read_conf_file(config_file, &g_cfg); cBYE(status);
  status = validate_config(g_cfg); cBYE(status);
  status = init_lua(); cBYE(status); // after read_conf_file
  // Must do Lua before C 
  // status = l_load_config(config_file); cBYE(status);
  status = l_update_config(); cBYE(status);
  status = update_config(); cBYE(status);
BYE:
  return status;
}
