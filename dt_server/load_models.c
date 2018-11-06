#include "dt_incs.h"
#include "dt_globals.h"
#include "auxil.h"
#include "init.h"
#include "load_models.h"
#include "get_from_lua.h"

#include "load_dt.h"
#include "load_rf.h"
#include "load_mdl.h"
extern DT_REC_TYPE *g_dt_map;  /* [g_num_dt_map] */
extern uint32_t g_num_dt_map; 
extern size_t g_len_dt_file; 

static bool
do_binary_files_exist(
    const char * const dt_dir,
    const char * const model_name
    )
{
  int status = 0;
  char *buf = NULL;
  // does directory exist
  if ( !isdir(dt_dir) ) { go_BYE(-1); }
  // do relevant binary files exist
  int buflen = strlen(dt_dir) + strlen(model_name) + 32;
  buf = malloc(buflen); return_if_malloc_failed(buf);
  memset(buf, '\0', buflen); 

  sprintf(buf, "%s/%s/_dt.bin", dt_dir, model_name); 
  if ( !isfile(buf) ) { go_BYE(-1); }

  sprintf(buf, "%s/%s/_rf.bin", dt_dir, model_name); 
  if ( !isfile(buf) ) { go_BYE(-1); }

  sprintf(buf, "%s/%s/_mdl.bin", dt_dir, model_name); 
  if ( !isfile(buf) ) { go_BYE(-1); }
BYE:
  if ( status < 0 ) { return false; } else { return true; }
}

int
load_models(
    const char * const dt_dir,
    const char * const model_name
    )
{
  int status = 0;
  char *buf = NULL;  int buflen = 0;
  if ( ( dt_dir == NULL ) || ( *dt_dir == '\0' ) ) { go_BYE(-1); }
  if ( ( model_name == NULL ) || ( *model_name == '\0' ) ) { go_BYE(-1); }
  if ( !do_binary_files_exist(dt_dir, model_name) ) {
    fprintf(stderr, "Creating binary files for model \n");
    go_BYE(-1);
  }
  //--------------------------------------------------------
  // dt, rf, mdl
  free_if_non_null(g_dt_feature_vector);  g_n_dt_feature_vector = 0;
  free_if_non_null(g_predictions); 
  rs_munmap(g_dt,  g_len_dt_file);  g_n_dt = 0;
  rs_munmap(g_rf,  g_len_rf_file);  g_n_rf = 0;
  rs_munmap(g_mdl, g_len_mdl_file); g_n_mdl = 0;

  buflen = strlen(dt_dir) + strlen(model_name) + 32;
  buf = malloc(buflen); return_if_malloc_failed(buf);
  memset(buf, '\0', buflen); 

  sprintf(buf, "%s/%s/_dt.bin", dt_dir, model_name); 
  status = load_dt(buf, &g_dt, &g_len_dt_file, &g_n_dt);
  cBYE(status);

  sprintf(buf, "%s/%s/_rf.bin", dt_dir, model_name); 
  status = load_rf(buf, &g_rf, &g_len_rf_file, &g_n_rf);
  cBYE(status);

  sprintf(buf, "%s/%s/_mdl.bin", dt_dir, model_name); 
  status = load_mdl(buf, &g_mdl, &g_len_mdl_file, &g_n_mdl);
  cBYE(status);

  g_predictions = malloc(g_n_mdl * sizeof(float));
  return_if_malloc_failed(g_predictions);

  status = get_num_features(&g_n_dt_feature_vector ); cBYE(status); 
  g_dt_feature_vector = malloc(g_n_dt_feature_vector * sizeof(float));
  return_if_malloc_failed(g_dt_feature_vector);
  //--------------------------------------------------------
BYE:
  free_if_non_null(buf);
  return status;
}

int
x_load_models(
    const char * const args
    )
{
  int status = 0;
  const char *dt_dir = NULL, *old_model_name = NULL;
  status = get_mdl_loc(&dt_dir, &old_model_name); cBYE(status);
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
  status = load_models(dt_dir, model_name); cBYE(status);
  // Inform Lua that model name has changed
  status = set_model_name(model_name); cBYE(status);
BYE:
  return status;
}
