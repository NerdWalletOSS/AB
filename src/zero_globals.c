#include "ab_incs.h"
#include "ab_globals.h"
#include "zero_globals.h"
#include "aux_zero.h"
#include "auxil.h"

//<hdr>
void
free_globals(
    void
    )
//</hdr>
{
  free_if_non_null(g_ss_response);  g_sz_ss_response = 0;

  if ( g_statsd_link != NULL ) { 
    statsd_finalize(g_statsd_link);
    g_statsd_link = NULL;
  }

  free_if_non_null(g_log_q);

  free_if_non_null(g_uuid);
 
  shutdown_curl(); // for g_ch and g_ss_ch

  free_if_non_null(g_devices);  g_n_devices = 0;

  if ( ( g_ua_to_dev_map != NULL ) && ( g_n_ua_to_dev_map != 0 ) ) {
    munmap(g_ua_to_dev_map, g_n_ua_to_dev_map);
  }
  
  if ( g_L != NULL ) { lua_close(g_L); g_L = NULL; }
}

int 
zero_globals(
    void
    )
{
  int status = 0;

  if ( AB_NUM_BINS > 32767 ) { go_BYE(-1); }
  //------------------------------
  g_port        = 0; 
  g_verbose     = false;

  g_log_port    = 0; 
  memset(g_log_server, '\0', AB_MAX_LEN_SERVER_NAME+1);
  memset(g_log_url, '\0', AB_MAX_LEN_URL+1);
  memset(g_log_health_url, '\0', AB_MAX_LEN_URL+1);

  g_ss_port    = 0; 
  memset(g_ss_server, '\0', AB_MAX_LEN_SERVER_NAME+1);
  memset(g_ss_url, '\0', AB_MAX_LEN_URL+1);
  memset(g_ss_health_url, '\0', AB_MAX_LEN_URL+1);

  memset(g_statsd_host,    '\0', AB_MAX_LEN_SERVER_NAME+1);
  memset(g_statsd_inc,     '\0', AB_MAX_LEN_URL+1);
  memset(g_statsd_timing,  '\0', AB_MAX_LEN_URL+1);
  g_statsd_port = 0;

  g_num_post_retries = 0;
  g_reload_on_startup = true;
  memset(g_default_url,  '\0', AB_MAX_LEN_REDIRECT_URL+1);

  g_uuid_len = AB_MAX_LEN_UUID; // default 
  g_uuid = malloc((AB_MAX_LEN_UUID+1));
  return_if_malloc_failed(g_uuid);
  memset(g_uuid, '\0',  AB_MAX_LEN_UUID+1);
  g_xy_guid = 0;

  g_ss_response = NULL;
  g_sz_ss_response = AB_MAX_LEN_SS_RESPONSE+1;;
  g_ss_response = malloc(g_sz_ss_response); // some initial start
  return_if_malloc_failed(g_ss_response);
  memset(g_ss_response, '\0', g_sz_ss_response);

  g_statsd_link  = NULL;

  g_sz_log_q    = AB_DEFAULT_N_LOG_Q;
  g_n_log_q     = 0;
  g_log_q       = NULL;
  g_q_rd_idx    = 0;
  g_q_wr_idx    = 0;

  g_uuid        = NULL;

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

  memset(g_redirect_url, '\0', AB_MAX_LEN_REDIRECT_URL+1);
  memset(g_err, '\0', AB_ERR_MSG_LEN+1);
  memset(g_buf, '\0', AB_ERR_MSG_LEN+1);
  memset(g_rslt, '\0', AB_MAX_LEN_RESULT+1);
  memset(g_mrslt, '\0', AB_MAX_LEN_RESULT+1);
  memset(g_curl_payload, '\0', AB_MAX_LEN_PAYLOAD+1);

  // g_con, g_mutex, .... 

  g_devices    = NULL;
  g_n_devices  = 0;
  g_device_idx = 0;
  g_other_idx  = 0;

  memset(g_ua_to_dev_map_file, '\0', AB_MAX_LEN_FILE_NAME+1);
  g_devices = NULL;  g_n_devices = 0;
  g_ua_to_dev_map = NULL;  
  g_n_ua_to_dev_map = 0;
  g_num_ua_to_dev_map = 0;

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

  g_L = NULL;
  // For Lua
  g_L = luaL_newstate(); if ( g_L == NULL ) { go_BYE(-1); }
  luaL_openlibs(g_L);  
  if ( ( luaL_loadfile(g_L, "ab.lua") ) || 
      ( lua_pcall(g_L, 0, 0, 0)) )   {
    go_BYE(-1);
  }

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

  g_log_ss_calls      = 0;
  g_log_ss_bad_code   = 0;
  g_log_ss_non_ascii  = 0;
  g_log_ss_null_data  = 0;
  g_log_ss_bad_json   = 0;
  g_log_ss_no_session = 0;

  g_log_missing_test_ab = 0;
  g_log_missing_test_xy = 0;

  g_log_no_test_name  = 0; // GetVariant
  g_log_no_test_names = 0; // GetVariants

  g_log_dropped_posts     = 0;
  g_log_posts             = 0;
  g_log_bad_posts         = 0;
  g_log_failed_posts      = 0;

  g_log_bad_uuid      = 0; 
  g_log_bad_test_type = 0; 
  g_log_bad_test_name = 0; 

  g_log_get_alt_variant_calls = 0;
  g_log_get_variant_calls     = 0;
  g_log_get_variants_calls    = 0;
  g_log_router_calls          = 0;
  g_log_bad_router_calls      = 0;
}
