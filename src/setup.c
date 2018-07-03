#include "ab_incs.h"
#include "ab_globals.h"
#include "zero_globals.h"
#include "init.h"
#include "update_config.h"
#include "l_update_config.h"
#include "l_load_config.h"
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
  status = init_lua(); cBYE(status);
  if ( ( config_file[0] == NULL ) || ( config_file[0] == '\0' ) ) { 
    go_BYE(-1); 
  }
  status = read_conf_file(config_file);
  status = l_load_config(config_file); cBYE(status);
  // IMPORTANT: Update lua before C: Order of following 2 lines matters
  status = l_update_config(); cBYE(status);
  status = update_config(); cBYE(status);
BYE:
  return status;
}
