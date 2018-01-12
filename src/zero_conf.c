#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "ab_constants.h"
#include "ab_types.h"
#include "ab_globals.h"
#include "macros.h"
#include "zero_conf.h"
#include "auxil.h"

int
zero_conf(
    void
    )
{
  int status = 0;

  g_t_start                = get_time_usec();

  g_port        = 0; 
  g_verbose     = false;
  g_log_csv     = false;

  g_log_port    = 0; // should be explicitly set
  memset(g_log_server, '\0', AB_MAX_LEN_SERVER_NAME+1);
  memset(g_log_url, '\0', AB_MAX_LEN_URL+1);
  memset(g_log_health_url, '\0', AB_MAX_LEN_URL+1);

  g_ss_port    = 0; // should be explicitly set
  memset(g_ss_server, '\0', AB_MAX_LEN_SERVER_NAME+1);
  memset(g_ss_url, '\0', AB_MAX_LEN_URL+1);
  memset(g_ss_health_url, '\0', AB_MAX_LEN_URL+1);

  memset(g_statsd_host,    '\0', AB_MAX_LEN_SERVER_NAME+1);
  memset(g_statsd_inc,     '\0', AB_MAX_LEN_URL+1);
  memset(g_statsd_timing,  '\0', AB_MAX_LEN_URL+1);
  g_statsd_port = 0;

  g_sz_log_q    = 0;
  g_num_post_retries = 0;
  g_sz_uuid_ht  = 0;
  g_reload_on_startup = true;
  memset(g_default_url,  '\0', AB_MAX_LEN_REDIRECT_URL+1);
  g_xy_guid = 0;
  g_test_uuid_len = false;
  g_uuid_len = AB_MIN_LEN_UUID; // default 
  if ( g_uuid_len > AB_MAX_LEN_UUID ) { go_BYE(-1); }
  //--------------
  g_ch             = NULL;
  g_curl_hdrs      = NULL;
  g_ss_ch         = NULL;
  g_ss_curl_hdrs  = NULL;
  //--------------
  g_devices   = NULL;
  g_n_devices = 0;

  memset(g_ua_to_dev_map_file, '\0', AB_MAX_LEN_FILE_NAME+1);
  g_devices = NULL;  g_n_devices = 0;
  g_ua_to_dev_map = NULL;  g_n_ua_to_dev_map = 0;

  g_L = NULL;

  // Put following in one time init TODO P3
  const char *str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ&=/:_-%,;[].?+() ";
  memset(g_valid_chars_in_url, '\0', 256);
  for ( char *cptr = (char *)str; *cptr != '\0'; cptr++ ) {
    g_valid_chars_in_url[(uint8_t)(*cptr)] = true;
  }
  // put above in one time init TODO P3
  // Internal log 
  g_log = NULL;
  g_log_idx = 0;

  g_log_no_user_agent  = 0;
  g_log_bad_user_agent = 0;
  g_log_router_calls   = 0;

  g_log_ss_calls      = 0;
  g_log_ss_non_ascii  = 0;
  g_log_ss_bad_code   = 0;
  g_log_ss_no_session = 0;
  g_log_ss_bad_json   = 0;

  g_log_missing_test_ab = 0;
  g_log_missing_test_xy = 0;

  g_log_no_test_name  = 0; // GetVariant
  g_log_no_test_names = 0; // GetVariants

  g_log_posts              = 0;
  g_log_bad_posts          = 0;
  g_log_failed_posts       = 0;

  g_log_bad_uuid      = 0; 
  g_log_bad_test_type = 0; 
  g_log_bad_test_name = 0; 

  g_num_get_variant_calls  = 0;
  g_num_bad_xy_calls       = 0;
BYE:
  return status;
}
