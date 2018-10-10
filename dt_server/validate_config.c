#include "dt_incs.h"
#include "dt_constants.h"
#include "dt_httpd_types.h"
#include "auxil.h"
#include "validate_config.h"

extern char g_valid_chars_in_url[256]; 

int
validate_config(
    CFG_TYPE cfg
    )
{
  int status = 0;
  char *buf = NULL;

  if ( ( cfg.port <= 0 ) ||  ( cfg.port >= 65536 ) ) { go_BYE(-1); }
  //---------------------------------
  if ( !isdir(cfg.dt_dir) ) { go_BYE(-1); }
  if ( cfg.model_name[0] == '\0' ) { go_BYE(-1); }
  int len = strlen(cfg.dt_dir) + strlen(cfg.model_name) + 8;
  buf = calloc(len, 1);
  sprintf(buf, "%s/%s", cfg.dt_dir, cfg.model_name);
  if ( !isdir(buf) ) { go_BYE(-1); }
  //---------------------------------
BYE:
  free_if_non_null(buf);
  return status;
}
