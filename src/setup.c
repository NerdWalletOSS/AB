#include "ab_incs.h"
#include "ab_globals.h"
#include "zero_globals.h"
#include "init.h"
#include "update_config.h"
#include "l_update_config.h"
#include "l_load_config.h"
#include "l_hard_code_config.h"
#include "hard_code_config.h"
#include "setup.h"
extern char g_config_file[AB_MAX_LEN_FILE_NAME+1];

int
setup(
    )
{
  int status = 0;

  free_globals(); 
  zero_globals();
  init_globals();
  status = init_lua(); cBYE(status);
  if ( g_config_file[0] == '\0' ) { 
    hard_code_config(); // only for testing 
    status = l_hard_code_config(); cBYE(status); // only for testing 
  }
  else {
    status = l_load_config(g_config_file); cBYE(status);
  }
  // IMPORTANT: Update lua before C: Order of following 2 lines matters
  status = l_update_config(); cBYE(status);
  status = update_config(); cBYE(status);
BYE:
  return status;
}
