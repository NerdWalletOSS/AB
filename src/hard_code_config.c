#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "hard_code_config.h"
void
hard_code_config(void)
{
  g_port = 8000;

  strcpy(g_log_server,  "localhost");
  g_log_port = 8004;
  strcpy(g_log_url, "Log");
  strcpy(g_log_health_url, "Ignore");

  strcpy(g_ss_server, "localhost");
  g_ss_port = 8024;
  strcpy(g_ss_url, "session");
  strcpy(g_ss_health_url, "api/v1/health_check");

  g_num_post_retries = 1;
  g_verbose = false;

  strcpy(g_statsd_host, "localhost");
  g_statsd_port = 8125;
  strcpy(g_statsd_inc,  "nw.metrics.ab.requests");
  strcpy(g_statsd_timing, "nw.metrics.ab.request_time");
  
  g_sz_log_q = 65536;
  
  strcpy(g_default_url, "localhost:8000/index.html");
  g_test_uuid_len = false;
  g_reload_on_startup = false;
  g_xy_guid = 1;
  g_uuid_len = 8;
  strcpy(g_ua_to_dev_map_file,  "opt/ab/ua_to_dev.bin");
}
