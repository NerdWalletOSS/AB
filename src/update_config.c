#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "zero_globals.h"
#include "init.h"
#include "update_config.h"
#include "load_lkp.h"
#include "load_user_agent_classifier.h"
#include "l_get_num_features.h"
#include "statsd.h"

#include "load_dt.h"
#include "load_rf.h"
#include "load_mdl.h"
#include "maxminddb.h"
#ifdef KAFKA
#include "kafka_close_conn.h"
#include "kafka_open_conn.h"
#include "kafka_add_to_queue.h"
#endif
#ifdef MAXMIND
extern MMDB_s g_mmdb; extern bool g_mmdb_in_use;
#endif
extern DT_REC_TYPE *g_dt_map; 
extern size_t g_len_dt_file; 
extern uint32_t g_num_dt_map; 

int
update_config(
    void
    )
{
  int status = 0;
  char *buf = NULL; 
  // sz_log_q
  free_if_non_null(g_log_q);
  if ( g_cfg.sz_log_q <= 0 ) { go_BYE(-1); }
#ifdef AB_AS_KAFKA
  g_log_q = malloc(g_cfg.sz_log_q * sizeof(void *)); 
  return_if_malloc_failed(g_log_q);
  memset(g_log_q, '\0', (g_cfg.sz_log_q * sizeof(void *)));
#else
  g_log_q = malloc(g_cfg.sz_log_q * sizeof(PAYLOAD_REC_TYPE)); 
  return_if_malloc_failed(g_log_q);
  memset(g_log_q, '\0', (g_cfg.sz_log_q * sizeof(PAYLOAD_REC_TYPE)));
#endif
  //---------------------------------------------------
  if ( g_statsd_link != NULL ) { 
    statsd_finalize(g_statsd_link);
    g_statsd_link = NULL;
  }
  if ( g_disable_sd ) {
    fprintf(stderr, "WARNING! Not logging to statsd \n");
  }
  else {
    g_statsd_link = statsd_init(g_cfg.statsd.server, g_cfg.statsd.port);
    if ( g_statsd_link == NULL ) { go_BYE(-1); }
  }
  
  STATSD_GAUGE("start_time", g_log_start_time);
  // log_server
  // log_port
  // log_url
  // log_health_url
  // dim_server
  // dim_port
  // dim_url
  // dim_health_url
  shutdown_curl();
  CURLcode res;
  res = curl_global_init(CURL_GLOBAL_DEFAULT);
  if ( res != CURLE_OK) {
    fprintf(stderr, "curl_global_init() failed: %s\n",
        curl_easy_strerror(res));
    go_BYE(-1); 
  }
  // Following for curl for logging GetVariant 
  if ( g_disable_lg ) { 
    fprintf(stderr, "WARNING! /GetVariant NOT being logged\n");
  }
  else {
    status = setup_curl("POST", NULL, "logger", g_cfg.logger.server, 
        g_cfg.logger.port, g_cfg.logger.url, g_cfg.logger.health_url, 
        AB_LOGGER_TIMEOUT_MS, &g_ch, &g_curl_hdrs);
    cBYE(status);
  }
  // num_post_retries, Nothing to do 
  // verbose, Nothing to do 
  // mysql_server For Lua
  // mysql_user For Lua
  // mysql_password For Lua
  // mysql_database For Lua
  // default_url, Nothing to do 
  // reload_on_startup, Nothing to do 

  free_if_non_null(g_uuid);
  if ( g_cfg.max_len_uuid > AB_MAX_LEN_UUID ) { go_BYE(-1); }
  if ( g_cfg.max_len_uuid < 1 ) { go_BYE(-1); }
  g_uuid = malloc(g_cfg.max_len_uuid+1);
  return_if_malloc_failed(g_uuid);

  status = load_user_agent_classifier(g_disable_ua,
    g_cfg.ua_dir, &g_justin_cat_other_id,
    &g_classify_ua_map, &g_len_classify_ua_file, &g_num_classify_ua_map, 
    &g_justin_cat_lkp, &g_n_justin_cat_lkp, 
    &g_os_lkp, &g_n_os_lkp, 
    &g_browser_lkp, &g_n_browser_lkp, 
    &g_device_type_lkp, &g_n_device_type_lkp);
  cBYE(status);
  if ( g_n_justin_cat_lkp < 1 ) { go_BYE(-1); }
  //--------------------------------------------------------
  // dt, rf, mdl
  free_if_non_null(g_dt_feature_vector); 
  free_if_non_null(g_predictions); 
  if ( g_dt  != NULL ) { rs_munmap(g_dt,  g_len_dt_file); }
  if ( g_rf  != NULL ) { rs_munmap(g_rf,  g_len_rf_file); }
  if ( g_mdl != NULL ) { rs_munmap(g_mdl, g_len_mdl_file); }
  if ( *g_cfg.dt_dir != '\0' ) { 

    // fprintf(stderr, "WARNING!!!! Not loading dt_dir \n"); 

    int buflen = strlen(g_cfg.dt_dir) + strlen(g_cfg.model_name) + 32;
    buf = malloc(buflen); return_if_malloc_failed(buf);
    memset(buf, '\0', buflen); 

    sprintf(buf, "%s/%s/_dt.bin", g_cfg.dt_dir, g_cfg.model_name); 
    status = load_dt(buf, &g_dt, &g_len_dt_file, &g_n_dt);
    cBYE(status);

    sprintf(buf, "%s/%s/_rf.bin", g_cfg.dt_dir, g_cfg.model_name); 
    status = load_rf(buf, &g_rf, &g_len_rf_file, &g_n_rf);
    cBYE(status);

    sprintf(buf, "%s/%s/_mdl.bin", g_cfg.dt_dir, g_cfg.model_name); 
    status = load_mdl(buf, &g_mdl, &g_len_mdl_file, &g_n_mdl);
    cBYE(status);

    g_predictions = malloc(g_n_mdl * sizeof(float));
    return_if_malloc_failed(g_predictions);

    status = l_get_num_features(&g_n_dt_feature_vector ); cBYE(status); 
    g_dt_feature_vector = malloc(g_n_dt_feature_vector * sizeof(float));
    return_if_malloc_failed(g_dt_feature_vector);
  }
  //--------------------------------------------------------

#ifdef MAXMIND
  if ( g_mmdb_in_use ) { 
    MMDB_close(&g_mmdb);
    g_mmdb_in_use = false;
  }
  g_mmdb_in_use = false;
  if ( *g_cfg.mmdb_file != '\0' ) { 
    status = MMDB_open(g_cfg.mmdb_file, 0, &g_mmdb); 
    if ( status != MMDB_SUCCESS ) { go_BYE(-1); }
    if ( status == MMDB_IO_ERROR ) { go_BYE(-1); }
    // FIX 2nd parameter TODO P1
    g_mmdb_in_use = true;
  }
#endif
#ifdef KAFKA
  kafka_close_conn();
  if ( !g_disable_kf ) { 
    status = kafka_open_conn(g_cfg.kafka); cBYE(status);
  }
#endif
  // ---------------------
BYE:
  free_if_non_null(buf);
  return status;
}
