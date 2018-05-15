#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "zero_globals.h"
#include "init.h"
#include "update_config.h"
#include "load_lkp.h"
#include "load_classify_ua_map.h"
#include "l_get_num_features.h"

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
  if ( g_cfg.sz_log_q > 0 ) { 
    g_log_q = malloc(g_cfg.sz_log_q * sizeof(PAYLOAD_TYPE)); 
    return_if_malloc_failed(g_log_q);
    memset(g_log_q, '\0', (g_cfg.sz_log_q * sizeof(PAYLOAD_TYPE)));
    g_n_log_q = 0;
  }
  //---------------------------------------------------

  // statsd.server 
  // statsd.port 
  if ( g_statsd_link != NULL ) { 
    statsd_finalize(g_statsd_link);
    g_statsd_link = NULL;
  }
  if ( ( *g_cfg.statsd.server == '\0' ) || ( g_cfg.statsd.port <= 0 ) ) {
    fprintf(stderr, "WARNING! Not logging to statsd \n");
  }
  else {
    g_statsd_link = statsd_init(g_cfg.statsd.server, g_cfg.statsd.port);
    if ( g_statsd_link == NULL ) { go_BYE(-1); }
  }
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
  if ( ( *g_cfg.logger.server == '\0' ) || ( *g_cfg.logger.url == '\0' ) ||
      ( ( g_cfg.logger.port <= 1 )  || ( g_cfg.logger.port >= 65535 ) ) ) {
    fprintf(stderr, "WARNING! /GetVariant NOT being logged\n");
  }
  else {
    status = setup_curl("POST", NULL, "logger", g_cfg.logger.server, 
        g_cfg.logger.port, g_cfg.logger.url, g_cfg.logger.health_url, 
        AB_LOGGER_TIMEOUT_MS, &g_ch, &g_curl_hdrs);
    cBYE(status);
  }
  // Following for curl  for Session Service
  if ( ( *g_cfg.ss.server == '\0' ) || ( *g_cfg.ss.url == '\0' ) ||
      ( ( g_cfg.ss.port <= 1 )  || ( g_cfg.ss.port >= 65535 ) ) ) {
    fprintf(stderr, "WARNING! SessionService not in use\n");
  }
  else {
    /* The get_or_create endpoint averages around 25ms response, with 
     * a 95th percentile of 30ms. The plain get endpoint would be 
     * expected to be faster -- Andrew Hollenbach */
    status = setup_curl("GET", g_ss_response, "ss", g_cfg.ss.server, 
        g_cfg.ss.port, g_cfg.ss.url, g_cfg.ss.health_url, 
        AB_SS_TIMEOUT_MS, &g_ss_ch, &g_ss_curl_hdrs);
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
  // xy_guid, Nothing to do 

  free_if_non_null(g_uuid);
  if ( g_cfg.max_len_uuid > AB_MAX_LEN_UUID ) { go_BYE(-1); }
  if ( g_cfg.max_len_uuid < 1 ) { go_BYE(-1); }
  g_uuid = malloc(g_cfg.max_len_uuid+1);
  return_if_malloc_failed(g_uuid);
  // justin cat file 
  free_if_non_null(g_justin_cat_lkp);  
  g_n_justin_cat_lkp = 0; 
  g_justin_cat_other_id = -1;
  if ( *g_cfg.justin_cat_file != '\0' ) { 
    status = load_lkp(g_cfg.justin_cat_file, &g_justin_cat_lkp, 
        &g_n_justin_cat_lkp);
    cBYE(status);
    for ( int i = 0; i < g_n_justin_cat_lkp; i++ ) { 
      if ( strcasecmp(g_justin_cat_lkp[i].name, "Other") == 0 ) {
        g_justin_cat_other_id = g_justin_cat_lkp[i].id;
      }
    }
  }
  //--------------------------------------------------------
  // os file 
  free_if_non_null(g_os_lkp);  
  g_n_os_lkp = 0; 
  if ( *g_cfg.os_file != '\0' ) { 
    status = load_lkp(g_cfg.os_file, &g_os_lkp, 
        &g_n_os_lkp);
    cBYE(status);
  }
  //--------------------------------------------------------
  // browser file 
  free_if_non_null(g_browser_lkp);  
  g_n_browser_lkp = 0; 
  if ( *g_cfg.browser_file != '\0' ) { 
    status = load_lkp(g_cfg.browser_file, &g_browser_lkp, 
        &g_n_browser_lkp);
    cBYE(status);
  }
  //--------------------------------------------------------
  // device_type file 
  free_if_non_null(g_device_type_lkp);  
  g_n_device_type_lkp = 0; 
  if ( *g_cfg.device_type_file != '\0' ) { 
    status = load_lkp(g_cfg.device_type_file, &g_device_type_lkp, 
        &g_n_device_type_lkp);
    cBYE(status);
  }
  //--------------------------------------------------------
  // ua_to_dev_map_file
  if ( ( g_classify_ua_map != NULL ) && ( g_len_classify_ua_file != 0 ) ) {
    munmap(g_classify_ua_map, g_len_classify_ua_file);
  }
  if ( *g_cfg.ua_to_dev_map_file != '\0' ) { 
    status = load_classify_ua_map(g_cfg.ua_to_dev_map_file, 
        &g_classify_ua_map, &g_len_classify_ua_file, &g_num_classify_ua_map);
    cBYE(status);
  }
  //--------------------------------------------------------
  // dt, rf, mdl
  free_if_non_null(g_dt_feature_vector); 
  free_if_non_null(g_predictions); 
  if ( g_dt  != NULL ) { rs_munmap(g_dt,  g_len_dt_file); }
  if ( g_rf  != NULL ) { rs_munmap(g_rf,  g_len_rf_file); }
  if ( g_mdl != NULL ) { rs_munmap(g_mdl, g_len_mdl_file); }
  if ( *g_cfg.dt_dir != '\0' ) { 
    fprintf(stderr, "WARNING!!!! Not loading dt_dir \n"); 
    /*
    int buflen = strlen(g_cfg.dt_dir) + strlen("dt.bin") + 4 ;
    if ( buflen > AB_MAX_LEN_FILE_NAME ) { go_BYE(-1); }
    buf = malloc(buflen); return_if_malloc_failed(buf);
    memset(buf, '\0', buflen); 
    sprintf(g_dt_file, "%s/dt.bin", g_cfg.dt_dir); 
    status = load_dt(g_dt_file, &g_dt, &g_len_dt_file, &g_n_dt);
    cBYE(status);
    status = load_rf(g_rf_file, &g_rf, &g_len_rf_file, &g_n_rf);
    cBYE(status);
    status = load_mdl(g_mdl_file, &g_mdl, &g_len_mdl_file, &g_n_mdl);
    cBYE(status);
    */
    g_n_mdl = 1 ; // TODO P0 DELETE DELETE DELETE 
    g_predictions = malloc(g_n_mdl * sizeof(float));

    free_if_non_null(g_dt_feature_vector); 
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
  if ( g_cfg.kafka.brokers[0] != '\0' ) { 
    // status = kafka_open_conn(g_cfg.kafka.topic, g_cfg.kafka.brokers); cBYE(status);
     kafka_open_conn(g_cfg.kafka);
     // sprintf(g_buf, "hey in hardcode for demo\0"); 
     // INDRAKEET TO DO take out null character if not needed. gcc complains
     // kafka_add_to_queue(g_buf);  
  }
#endif
  // ---------------------
  // INDRAJEET: PUT IN STUFF FOR LUA 

BYE:
  free_if_non_null(buf);
  return status;
}
