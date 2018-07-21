#include "ab_incs.h"
#include "ab_constants.h"
#include "auxil.h"
#include "mmap.h"
#include <jansson.h>
#include "read_conf_file.h"
#include "zero_globals.h"

extern  bool g_disable_dt; // set to false if no decision tree
extern bool g_disable_ua; // set to false if no user agent classifier
extern bool g_disable_ip; // set to false if no MaxMind Database
extern bool g_disable_sd; // set to false if no statsd server
extern bool g_disable_wa; // disable WebApp
extern bool g_disable_lg; // disable Logger
extern bool g_disable_kf; // disable Kafka

static int 
get_server(
    json_t *root,
    const char *service,
    SERVICE_TYPE *ptr_ss
    )
{
  int status = 0;
  status = get_int(root, service, "PORT", "VALUE", &(ptr_ss->port));
  status = get_string(root, service, "SERVER", "VALUE", 
      AB_MAX_LEN_SERVER_NAME, ptr_ss->server);
  status = get_string(root, service, "URL", "VALUE", 
      AB_MAX_LEN_URL, ptr_ss->url);
  status = get_string(root, service, "HEALTH_URL", "VALUE", 
      AB_MAX_LEN_URL, ptr_ss->health_url);
  return status;
}

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
  if ( X == NULL ) { go_BYE(-1); }
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

  json_t *j_ip = json_object_get(root, "IP_ADDRESS_CLASSIFIER");
  if ( j_ip == NULL ) { 
    g_disable_ip = true;
  } 
  else {
    g_disable_ip = false;
    status = get_string(j_ip, "MMDB_FILE","VALUE", NULL, 
        AB_MAX_LEN_FILE_NAME, ptr_cfg->mmdb_file);
  }
  //----------------------
  json_t *j_ua = json_object_get(root, "USER_AGENT_CLASSIFIER");
  if ( j_ua == NULL ) { 
    g_disable_ua = true;
  } 
  else {
    g_disable_ua = false;
    status = get_string(j_ua, "UA_DIR","VALUE", NULL, AB_MAX_LEN_FILE_NAME, 
        ptr_cfg->ua_dir);
  }
  //----------------------
  json_t *j_dt = json_object_get(root, "DT");
  if ( j_dt == NULL ) { 
    g_disable_dt = true;
  } 
  else {
    g_disable_dt = false;
    status = get_string(j_dt, "DT_DIR","VALUE", NULL, AB_MAX_LEN_FILE_NAME, 
        ptr_cfg->dt_dir);
    status = get_string(j_dt, "MODEL_NAME","VALUE", NULL, 
        AB_MAX_LEN_FILE_NAME, ptr_cfg->model_name);
  }
  //----------------------
  json_t *j_ab = json_object_get(root, "AB");
  if ( j_ab == NULL ) { go_BYE(-1); }

  status = get_int(j_ab, "SZ_LOG_Q", "VALUE", NULL, &(ptr_cfg->sz_log_q));

  status = get_int(j_ab, "PORT", "VALUE", NULL, &(ptr_cfg->port));

  status = get_int(j_ab, "MAX_LEN_UUID", "VALUE", NULL, 
      &(ptr_cfg->max_len_uuid));

  status = get_bool(j_ab, "VERBOSE", "VALUE", NULL, &(ptr_cfg->verbose));

  status = get_int(j_ab, "LOGGER", "NUM_POST_RETRIES", "VALUE", 
      &(ptr_cfg->num_post_retries));

  status = get_string(j_ab, "DEFAULT_URL", "VALUE", NULL, AB_MAX_LEN_URL,
      ptr_cfg->default_url);

  //--- START: Statsd keys
  json_t *j_sd = json_object_get(j_ab, "STATSD");
  if ( j_sd == NULL ) { 
    g_disable_sd = true;
  } 
  else {
    g_disable_sd = false;
    status = get_string(j_ab, "STATSD", "STATSD_COUNT", "VALUE", 
        AB_MAX_LEN_STATSD_KEY, ptr_cfg->statsd_keys.count);
    status = get_string(j_ab, "STATSD", "STATSD_INC", "VALUE", 
        AB_MAX_LEN_STATSD_KEY, ptr_cfg->statsd_keys.inc);
    status = get_string(j_ab, "STATSD", "STATSD_GAUGE", "VALUE", 
        AB_MAX_LEN_STATSD_KEY, ptr_cfg->statsd_keys.gauge);
    status = get_string(j_ab, "STATSD", "STATSD_TIMING", "VALUE", 
        AB_MAX_LEN_STATSD_KEY, ptr_cfg->statsd_keys.timing);
    get_server(j_ab, "STATSD", &(ptr_cfg->statsd));
  }

  //--- STOP : Statsd keys
  json_t *j_wa = json_object_get(j_ab, "WEBAPP");
  if ( j_wa == NULL ) { 
    g_disable_wa = true;
  } 
  else {
    g_disable_wa = false;
    get_server(j_ab, "WEBAPP", &(ptr_cfg->webapp));
  }

  json_t *j_lg = json_object_get(j_ab, "WEBAPP");
  if ( j_lg == NULL ) { 
    g_disable_lg = true;
  } 
  else {
    g_disable_lg = false;
    get_server(j_ab, "LOGGER", &(ptr_cfg->logger));
  }
  //-------------------------------------
  json_t *j_kf = json_object_get(j_ab, "KAFKA");
  if ( j_kf == NULL ) { 
    g_disable_kf = true;
  } 
  else {
    g_disable_kf = false;
    get_string(j_kf, "BROKERS", "VALUE", NULL, AB_MAX_LEN_KAFKA_PARAM,
        ptr_cfg->kafka.brokers);
    get_string(j_kf, "TOPIC", "VALUE", NULL, AB_MAX_LEN_KAFKA_PARAM,
        ptr_cfg->kafka.topic);
    get_string(j_kf, "RETRIES", "VALUE", NULL, AB_MAX_LEN_KAFKA_PARAM,
        ptr_cfg->kafka.retries);
    get_string(j_kf, "MAX_BUFFERING_TIME", "VALUE", NULL, AB_MAX_LEN_KAFKA_PARAM,
        ptr_cfg->kafka.max_buffering_time);
  }
  //-------------------------------------
  json_t *j_my = json_object_get(j_ab, "MYSQL");
  if ( j_my == NULL ) { go_BYE(-1); }
  get_string(j_my, "SERVER", "VALUE", NULL, AB_MAX_LEN_MYSQL_PARAM,
      ptr_cfg->mysql.host);
  get_string(j_my, "USER", "VALUE", NULL, AB_MAX_LEN_MYSQL_PARAM,
      ptr_cfg->mysql.user);
  get_string(j_my, "PASSWORD", "VALUE", NULL, AB_MAX_LEN_MYSQL_PARAM,
      ptr_cfg->mysql.pass);
  get_string(j_my, "DATABASE", "VALUE", NULL, AB_MAX_LEN_MYSQL_PARAM,
      ptr_cfg->mysql.db);
  get_int(j_my, "PORT", "VALUE", NULL, &(ptr_cfg->mysql.port));
  //-------------------------------------

  json_decref(root);
BYE:
  free_if_non_null(cbuf);
  rs_munmap(X, nX);
  return status;
}
