
#include "dt_incs.h"
#include "auxil.h"
#include "init.h"
#include "load_models.h"
extern DT_INTERPRETER_TYPE *g_interp;
int
x_load_models(
    const char * const args
    )
{
  int status = 0;
  char *buf = NULL;
#ifdef TODO
  const char *old_model_dir = NULL, *old_model_name = NULL;
  status = get_mdl_loc(&old_model_dir, &old_model_name); cBYE(status);

  char model_name[DT_MAX_LEN_FILE_NAME+1];
  memset(model_name, '\0', DT_MAX_LEN_FILE_NAME+1);
  status = extract_name_value(args, "ModelName=", '&', model_name, 
      DT_MAX_LEN_FILE_NAME); cBYE(status);
  // Check that model name is good
  mcr_chk_non_null(model_name);
  for ( char *cptr = model_name; *cptr != '\0'; cptr++ ) { 
    if ( !isascii(*cptr) ) { go_BYE(-1); }
    if ( ( !isalnum(*cptr) ) && ( *cptr != '_' ) ) { go_BYE(-1); }
  }
  //----------------------------------------

  bufsz = strlen(dt_dir) + strlen(model_name) + 8;
  buf = malloc(bufsz); return_if_malloc_failed(buf);
  sprintf(buf, "%s/%s", dt_dir, model_name);
  status = load_models(buf, &g_interp); cBYE(status);
  // Inform Lua that model name has changed
  status = set_model_name(model_name); cBYE(status);
#endif
BYE:
  free_if_non_null(buf);
  return status;
}
