#include "dt_incs.h"
#include "dt_constants.h"
#include "dt_httpd_types.h"
#include "auxil.h"
#include "mmap.h"
#include "read_conf_file.h"
#include "zero_globals.h"

int
get_string(
    json_t *root,
    const char *key1,
    const char *key2,
    const char *key3,
    size_t maxlen,
    char *dst
    )
{
  int status = 0;
  json_t *handle = NULL;

  if ( root == NULL ) { go_BYE(-1); }
  if ( key1 == NULL ) { go_BYE(-1); }
  handle = json_object_get(root, key1);
  // fprintf(stderr, " TODO DELETE %s:", key1);
  if ( handle == NULL ) { go_BYE(-1); }
  if ( ( key2 != NULL ) && ( *key2 != '\0' ) ) { 
    handle = json_object_get(handle, key2);
    // fprintf(stderr, "%s:", key2);
  }
  if ( ( key3 != NULL ) && ( *key3 != '\0' ) ) { 
    handle = json_object_get(handle, key3);
    // fprintf(stderr, "%s:", key3);
  }
  if ( handle == NULL ) { 
    fprintf(stderr, "Could not find %s", key1); 
    if ( key2 != NULL ) { fprintf(stderr, ":%s", key2); }
    if ( key3 != NULL ) { fprintf(stderr, ":%s", key3); }
    fprintf(stderr, "\n");
    go_BYE(-1); 
  }
  const char *X = json_string_value(handle);
  if ( X == NULL ) { 
    go_BYE(-1); 
  }
  if ( strlen(X) > maxlen ) { go_BYE(-1); }
  strcpy(dst, X);
  // fprintf(stderr, "%s", X);
BYE:
  // fprintf(stderr, "\n");
  return status;
}

int
get_bool(
    json_t *root,
    const char *key1,
    const char *key2,
    const char *key3,
    bool *ptr_rslt
    )
{
  int status = 0;
  char buf[32];
  memset(buf, '\0', 32);
  status = get_string(root, key1, key2, key3, 31, buf); cBYE(status);
  if ( strcasecmp(buf, "true") == 0 ) {
    *ptr_rslt = true;
  }
  else if ( strcasecmp(buf, "false") == 0 ) {
    *ptr_rslt = false;
  }
  else {
    go_BYE(-1);
  }
BYE:
  return status;
}

int
get_int(
    json_t *root,
    const char *key1,
    const char *key2,
    const char *key3,
    int32_t *ptr_rslt
    )
{
  int status = 0;
  char buf[32];
  memset(buf, '\0', 32);
  status = get_string(root, key1, key2, key3, 31, buf); 
  if ( status < 0 ) { 
    fprintf(stderr, "Could not find %s", key1); 
    if ( key2 != NULL ) { fprintf(stderr, ":%s", key2); }
    if ( key3 != NULL ) { fprintf(stderr, ":%s", key3); }
    fprintf(stderr, "\n");
    go_BYE(-1); 
  }
  status = stoI4(buf, ptr_rslt); cBYE(status);
BYE:
  return status;
}

int
read_conf_file(
    const char *const file_name,
    CFG_TYPE *ptr_cfg
    )
{
  int status = 0;
  char *cbuf = NULL; int buflen = 0;
  char *X = NULL; size_t nX = 0;

  if ( ( file_name == NULL ) || ( file_name[0] == '\0' ) ) { 
    go_BYE(-1); 
  }
  if ( !isfile(file_name) ) { go_BYE(-1); }
  // read file name into buffer
  status = rs_mmap(file_name, &X, &nX, 0); cBYE(status);
  buflen = nX+1;
  cbuf = calloc(buflen, 1); return_if_malloc_failed(cbuf);
  memcpy(cbuf, X, nX); 
  rs_munmap(X, nX);

  // read buffer into JSON
  json_t *root = NULL;
  json_error_t error;
  root = json_loads(cbuf, 0, &error);
  if ( root == NULL ) { go_BYE(-1); }

  //----------------------
  json_t *j_dt = json_object_get(root, "DT");
  status = get_string(j_dt, "DT_DIR","VALUE", NULL, DT_MAX_LEN_FILE_NAME, 
      ptr_cfg->dt_dir);
  status = get_string(j_dt, "MODEL_NAME","VALUE", NULL, 
      DT_MAX_LEN_FILE_NAME, ptr_cfg->model_name);
  json_decref(root);
BYE:
  free_if_non_null(cbuf);
  rs_munmap(X, nX);
  return status;
}
