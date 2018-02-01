#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "hard_code_config.h"
void
hard_code_config(void)
{
  g_cfg.port = 8000;
  g_cfg.verbose = false;

  strcpy(g_cfg.logger.server,  "localhost");
  g_cfg.logger.port = 8004;
  strcpy(g_cfg.logger.url, "Log");
  strcpy(g_cfg.logger.health_url, "Ignore");

  strcpy(g_cfg.ss.server, "localhost");
  g_cfg.ss.port = 8024;
  strcpy(g_cfg.ss.url, "session");
  strcpy(g_cfg.ss.health_url, "api/v1/health_check");

  strcpy(g_cfg.statsd.server, "localhost");
  g_cfg.statsd.port = 8125;
  
  g_cfg.sz_log_q = 65536;
  g_cfg.num_post_retries = 1;

  strcpy(g_cfg.default_url, "localhost:8000/index.html");
  g_cfg.uuid_len = 8;
  strcpy(g_cfg.ua_to_dev_map_file,  "opt/ab/ua_to_dev.bin");
}
