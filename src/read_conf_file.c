#include "ab_incs.h"
#include "ab_constants.h"
#include "auxil.h"
#include "mmap.h"
#include <err.h>
#include <event2/keyvalq_struct.h>
#include <event.h>
#include <evhttp.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <jansson.h>

#ifdef STAND_ALONE
CFG_TYPE g_cfg;
#endif

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

  handle = json_object_get(root, key1);
  if ( handle == NULL ) { go_BYE(-1); }
  if ( ( key2 != NULL ) && ( *key2 != '\0' ) ) { 
    handle = json_object_get(handle, key2);
  }
  if ( ( key3 != NULL ) && ( *key3 != '\0' ) ) { 
    handle = json_object_get(handle, key3);
  }
  if ( handle == NULL ) { go_BYE(-1); }
  const char *X = json_string_value(handle);
  if ( X == NULL ) { go_BYE(-1); }
  if ( strlen(X) > AB_MAX_LEN_FILE_NAME ) { go_BYE(-1); }
  strcpy(dst, X);
  printf("%s\n", X);
BYE:
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
  status = get_string(root, key1, key2, key3, 31, buf); cBYE(status);
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
  int32_t ival;

  // read file name into buffer
  status = rs_mmap(file_name, &X, &nX, 0); cBYE(status);
  buflen = nX+1;
  cbuf = calloc(buflen, 1); return_if_malloc_failed(cbuf);
  memcpy(cbuf, X, nX); 
  rs_munmap(X, nX);

  // read buffer into JSON
  json_t *root = NULL;
  json_t *val  = NULL;
  json_error_t error;
  root = json_loads(cbuf, 0, &error);
  if ( root == NULL ) { go_BYE(-1); }

  json_t *j_dt = json_object_get(root, "DT");
  if ( j_dt == NULL ) { go_BYE(-1); }

  status = get_string(j_dt, "DT_DIR","VALUE", NULL, AB_MAX_LEN_FILE_NAME, 
      ptr_cfg->dt_dir);
  status = get_string(j_dt, "MODEL_NAME","VALUE", NULL, 
      AB_MAX_LEN_FILE_NAME, ptr_cfg->model_name);
  //----------------------
  json_t *j_ab = json_object_get(root, "AB");
  if ( j_ab == NULL ) { go_BYE(-1); }

  status = get_int(j_ab, "SZ_LOG_Q", "VALUE", NULL, &(ptr_cfg->sz_log_q));

  status = get_int(j_ab, "PORT", "VALUE", NULL, &ival);
  if ( ( ival < 0 ) || ( ival >= 65535 ) ) { go_BYE(-1); }
  ptr_cfg->port = ival;

  status = get_bool(j_ab, "VERBOSE", "VALUE", NULL, &(ptr_cfg->verbose));

  status = get_int(j_ab, "LOGGER", "NUM_POST_RETRIES", "VALUE", 
      &(ptr_cfg->num_post_retries));

  status = get_string(j_ab, "DEFAULT_URL", "VALUE", NULL, AB_MAX_LEN_URL,
      ptr_cfg->default_url);

  status = get_string(j_ab, "MMDB_FILE", "VALUE", NULL, AB_MAX_LEN_URL,
      ptr_cfg->mmdb_file);
  //--- START: Statsd
  status = get_string(j_ab, "STATSD", "STATSD_COUNT", "VALUE", 
      AB_MAX_LEN_STATSD_KEY, ptr_cfg->statsd_count);
  status = get_string(j_ab, "STATSD", "STATSD_INC", "VALUE", 
      AB_MAX_LEN_STATSD_KEY, ptr_cfg->statsd_inc);
  status = get_string(j_ab, "STATSD", "STATSD_GAUGE", "VALUE", 
      AB_MAX_LEN_STATSD_KEY, ptr_cfg->statsd_gauge);
  status = get_string(j_ab, "STATSD", "STATSD_TIMING", "VALUE", 
      AB_MAX_LEN_STATSD_KEY, ptr_cfg->statsd_timing);

  //--- STOP : Statsd

  json_decref(root);
BYE:
  free_if_non_null(cbuf);
  rs_munmap(X, nX);
  return status;
}
int
validate_config(
    CFG_TYPE g_cfg
    )
{
  int status = 0;
BYE:
  return status;
}
#ifdef STAND_ALONE
int
main()
{
  read_conf_file("ab.conf.json", &g_cfg);
  validate_config(g_cfg);
  fprintf(stderr, "SUCCESS\n");
}

#endif
