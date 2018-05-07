#include "ab_incs.h"
#include "ab_globals.h"
#include "zero_globals.h"
#include "aux_zero.h"
#include "auxil.h"
#include "maxminddb.h"
#include "dt_types.h"
extern MMDB_s g_mmdb; extern bool g_mmdb_in_use;
extern DT_REC_TYPE *g_dt_map;
extern size_t g_len_dt_file;
extern uint32_t g_num_dt_map;

//<hdr>
void
free_globals(
    void
    )
  //</hdr>
{
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) {
    free_test(i); 
  }
  free_if_non_null(g_ss_response);  g_sz_ss_response = 0;

  if ( g_statsd_link != NULL ) {
    statsd_finalize(g_statsd_link);
    g_statsd_link = NULL;
  }

  free_if_non_null(g_log_q); g_n_log_q = 0;
  g_q_rd_idx = 0; g_q_wr_idx = 0;

  free_if_non_null(g_uuid);

  shutdown_curl(); // for g_ch and g_ss_ch

  free_if_non_null(g_justin_cat_lkp);  g_n_justin_cat_lkp = 0;
  free_if_non_null(g_os_lkp);  g_n_os_lkp = 0;
  free_if_non_null(g_browser_lkp);  g_n_justin_cat_lkp = 0;
  free_if_non_null(g_device_type_lkp);  g_n_device_type_lkp = 0;
  free_if_non_null(g_referer_class_lkp);  g_n_referer_class_lkp = 0;

  if ( ( g_classify_ua_map != NULL ) && ( g_len_classify_ua_file != 0 ) ) {
    munmap(g_classify_ua_map, g_len_classify_ua_file);
    g_num_classify_ua_map = 0;
  }
  if ( ( g_dt != NULL ) && ( g_len_dt_file > 0 ) ) { 
    munmap(g_dt, g_len_dt_file); g_n_dt = 0;
  }
  if ( ( g_rf != NULL ) && ( g_len_rf_file > 0 ) ) { 
    munmap(g_rf, g_len_rf_file); g_n_rf = 0;
  }
  if ( ( g_mdl != NULL ) && ( g_len_mdl_file > 0 ) ) { 
    munmap(g_mdl, g_len_mdl_file); g_n_mdl = 0;
  }

  if ( g_mmdb_in_use ) { MMDB_close(&g_mmdb); g_mmdb_in_use = false; }
  if ( !g_disable_lua ) { 
    if ( g_L    != NULL ) { lua_close(g_L);    g_L    = NULL; }
    if ( g_L_DT != NULL ) { lua_close(g_L_DT); g_L_DT = NULL; }
  }

  free_if_non_null(g_predictions); g_n_mdl = 0;

  kafka_close_conn();
}

int
zero_globals(
    void
    )
{
  int status = 0;

  if ( sizeof(UA_REC_TYPE) != (sizeof(uint64_t)+ (4*sizeof(uint8_t)) ) ) {
    go_BYE(-1);
  }
  //------------------------------
  g_cfg.port        = 0;
  g_cfg.verbose     = false;

  g_cfg.logger.port    = 0;
  memset(g_cfg.logger.server, '\0', AB_MAX_LEN_SERVER_NAME+1);
  memset(g_cfg.logger.url, '\0', AB_MAX_LEN_URL+1);
  memset(g_cfg.logger.health_url, '\0', AB_MAX_LEN_URL+1);

  g_cfg.ss.port    = 0;
  memset(g_cfg.ss.server, '\0', AB_MAX_LEN_SERVER_NAME+1);
  memset(g_cfg.ss.url, '\0', AB_MAX_LEN_URL+1);
  memset(g_cfg.ss.health_url, '\0', AB_MAX_LEN_URL+1);

  g_cfg.statsd.port = 0;
  memset(g_cfg.statsd.server,    '\0', AB_MAX_LEN_SERVER_NAME+1);

  g_cfg.num_post_retries = 0;
  memset(g_cfg.default_url,  '\0', AB_MAX_LEN_REDIRECT_URL+1);

  g_cfg.uuid_len = AB_MAX_LEN_UUID; // default
  g_uuid = malloc(AB_MAX_LEN_UUID+1);
  return_if_malloc_failed(g_uuid);
  memset(g_uuid, '\0',  AB_MAX_LEN_UUID+1);
  g_xy_guid = 0;

  memset(g_cfg.ua_to_dev_map_file, '\0', AB_MAX_LEN_FILE_NAME+1);
  memset(g_cfg.justin_cat_file, '\0', AB_MAX_LEN_FILE_NAME+1);
  memset(g_cfg.os_file, '\0', AB_MAX_LEN_FILE_NAME+1);
  memset(g_cfg.browser_file, '\0', AB_MAX_LEN_FILE_NAME+1);
  memset(g_cfg.device_type_file, '\0', AB_MAX_LEN_FILE_NAME+1);

  memset(g_cfg.dt_dir, '\0', AB_MAX_LEN_FILE_NAME+1);
  memset(g_dt_file, '\0', AB_MAX_LEN_FILE_NAME+1);
  memset(g_rf_file, '\0', AB_MAX_LEN_FILE_NAME+1);
  memset(g_mdl_file, '\0', AB_MAX_LEN_FILE_NAME+1);

  memset(g_cfg.mmdb_file, '\0', AB_MAX_LEN_FILE_NAME+1);

  g_ss_response = NULL;
  g_sz_ss_response = AB_MAX_LEN_SS_RESPONSE+1;;
  g_ss_response = malloc(g_sz_ss_response); // some initial start
  return_if_malloc_failed(g_ss_response);
  memset(g_ss_response, '\0', g_sz_ss_response);

  g_statsd_link  = NULL;

  g_cfg.sz_log_q    = AB_DEFAULT_N_LOG_Q;
  g_n_log_q     = 0;
  g_log_q       = NULL;
  g_q_rd_idx    = 0;
  g_q_wr_idx    = 0;

  memset(g_my_name, '\0', AB_MAX_LEN_HOSTNAME+1);
  status = gethostname(g_my_name, AB_MAX_LEN_HOSTNAME);
  cBYE(status);

  g_halt = false;

  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++ ) {
    status = zero_test(i); cBYE(status);
  }

  g_seed1 = 961748941; // large prime number
  g_seed2 = 982451653; // some other large primenumber
  spooky_init(&g_spooky_state, g_seed1, g_seed2);
  srand48(g_seed1);

  g_ch           = NULL;
  g_curl_hdrs    = NULL;
  g_ss_ch        = NULL;
  g_ss_curl_hdrs = NULL;

#ifdef NW_SPECIFIC
  memset(g_nw_x_caller_client_id, '\0', AB_MAX_LEN_HDR_VAL+1);
  memset(g_nw_x_cookie_id, '\0', AB_MAX_LEN_HDR_VAL+1);
#endif
  memset(g_redirect_url, '\0', AB_MAX_LEN_REDIRECT_URL+1);
  memset(g_err, '\0', AB_ERR_MSG_LEN+1);
  memset(g_buf, '\0', AB_ERR_MSG_LEN+1);
  memset(g_rslt, '\0', AB_MAX_LEN_RESULT+1);
  memset(g_mrslt, '\0', AB_MAX_LEN_RESULT+1);
  memset(g_curl_payload, '\0', AB_MAX_LEN_PAYLOAD+1);

  // g_con, g_mutex, ....

  g_justin_cat_other_id = -1;
  g_justin_cat_lkp  = NULL; g_n_justin_cat_lkp  = 0;
  g_os_lkp          = NULL; g_n_os_lkp          = 0;
  g_browser_lkp     = NULL; g_n_browser_lkp     = 0;
  g_device_type_lkp = NULL; g_n_device_type_lkp = 0;
  g_referer_class_lkp  = NULL; g_n_referer_class_lkp  = 0;

  g_classify_ua_map = NULL;
  g_len_classify_ua_file = 0;
  g_num_classify_ua_map = 0;

  g_dt_feature_vector = NULL;
  g_n_dt_feature_vector = 0;

  // TODO Check with Braad that this is good
  const char *url_str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ&=/:_-%,;[].?+() ";
  memset(g_valid_chars_in_url, '\0', 256);
  for ( char *cptr = (char *)url_str; *cptr != '\0'; cptr++ ) {
    g_valid_chars_in_url[(uint8_t)(*cptr)] = true;
  }

  // TODO Check with Braad that this is good
  const char *ua_str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ&=/:_-%,;[].?+() ";
  memset(g_valid_chars_in_ua, '\0', 256);
  for ( char *cptr = (char *)ua_str; *cptr != '\0'; cptr++ ) {
    g_valid_chars_in_ua[(uint8_t)(*cptr)] = true;
  }

  const char *arg_str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ&=";
  memset(g_valid_chars_in_ab_args, '\0', 256);
  for ( char *cptr = (char *)arg_str; *cptr != '\0'; cptr++ ) {
    g_valid_chars_in_ab_args[(uint8_t)(*cptr)] = true;
  }

  g_mmdb_in_use = false;
  memset(&g_maxmind, '\0', sizeof(MAXMIND_REC_TYPE));
  g_L    =  NULL;
  g_L_DT = NULL;
  g_dt  = NULL; g_n_dt = 0;
  g_rf  = NULL; g_n_rf = 0;
  g_mdl = NULL; g_n_mdl = 0;
  g_predictions = NULL;

  // For Kafka
  g_ignore_kafka_errors = false; 
  g_rk = NULL;         /* Producer instance handle */
  g_rkt = NULL;  /* Topic object */
  g_kafka_conf = NULL;  /* Temporary configuration object */
  memset(g_errstr, '\0', 512);
  //---------------------------

  zero_log();
BYE:
  return status;
}

void
zero_log()
{
  g_log_start_time  = get_time_usec();

  g_log_no_user_agent  = 0;
  g_log_bad_user_agent = 0;
  g_log_bad_ab_args    = 0;

  g_log_ss_calls      = 0;
  g_log_ss_bad_code   = 0;
  g_log_ss_timeout    = 0;
  g_log_ss_non_ascii  = 0;
  g_log_ss_null_data  = 0;
  g_log_ss_bad_json   = 0;
  g_log_ss_no_session = 0;
  g_log_ss_bad_calls  = 0;

  g_log_missing_test  = 0;

  g_log_no_test_name  = 0; // GetVariant
  g_log_no_test_names = 0; // GetVariants
  g_log_no_uuid       = 0;
  g_log_no_test_type  = 0;
  g_log_no_tracer     = 0;

  g_log_dropped_posts     = 0;
  g_log_posts             = 0;
  g_log_bad_posts         = 0;
  g_log_failed_posts      = 0;

  g_log_bad_uuid      = 0;
  g_log_bad_test_type = 0;
  g_log_bad_test_name = 0;
  g_log_bad_tracer    = 0;

  g_log_get_alt_variant_calls = 0;
  g_log_get_variant_calls     = 0;
  g_log_get_variants_calls    = 0;
  g_log_router_calls          = 0;
  g_log_bad_router_calls      = 0;

  g_log_num_probes     = 0;
}
