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
  g_cfg.xy_guid = 1;

  strcpy(g_cfg.ua_to_dev_map_file,  "/opt/ab/ua_to_dev.bin");
  strcpy(g_cfg.justin_cat_file,  "/opt/ab/justin_cat.csv");
  strcpy(g_cfg.os_file,  "/opt/ab/os.csv");
  strcpy(g_cfg.browser_file,  "/opt/ab/browser.csv");
  strcpy(g_cfg.device_type_file,  "/opt/ab/device_type.csv");

  strcpy(g_cfg.avg_fico_per_zip_file,  "/opt/avg_fico_per_zip.lua");
  strcpy(g_cfg.avg_zill_per_zip_file, "/opt/avg_zill_per_zip.lua");
  strcpy(g_cfg.referer_class_file,  "/opt/ab/referer_class.csv");

  // strcpy(g_cfg.dt_feature_file,  "/opt/ab/dt_feature.csv");
  // strcpy(g_cfg.dt_file,  "/opt/ab/dt.bin");

  strcpy(g_cfg.mmdb_file,  "/opt/ab/GeoIP2-City_2017_12_08.mmdb");
}
