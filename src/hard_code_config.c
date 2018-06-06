#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "hard_code_config.h"
#include "get_test_idx.h"
#ifdef KAFKA
#include "kafka_add_to_queue.h"
#endif

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

  strcpy(g_cfg.webapp.server, "localhost");
  g_cfg.webapp.port = 8080;
  strcpy(g_cfg.webapp.health_url, "AB/php/index.html");

  strcpy(g_cfg.statsd.server, "localhost");
  g_cfg.statsd.port = 8125;

  g_cfg.sz_log_q = 131072;
  g_cfg.num_post_retries = 1;

  strcpy(g_cfg.default_url, "http://localhost:8080/AB/test_webapp/index0.html");
  g_cfg.max_len_uuid = 47;
  g_cfg.xy_guid = 1;

  // START: For user agent classifier
  strcpy(g_cfg.ua_to_dev_map_file,  "/opt/ab/ua_to_dev.bin");
  strcpy(g_cfg.justin_cat_file,  "/opt/ab/justin_cat.csv");
  strcpy(g_cfg.os_file,  "/opt/ab/os.csv");
  strcpy(g_cfg.browser_file,  "/opt/ab/browser.csv");
  strcpy(g_cfg.device_type_file,  "/opt/ab/device_type.csv");
  strcpy(g_cfg.ua_model_coeff_file,  "/opt/ab/model_coefficients.csv");
  strcpy(g_cfg.ua_category_intercept_file,  "/opt/ab/category_intercept.csv");
  strcpy(g_cfg.ua.ua_to_dev_map_file,  "/opt/ab/ua_to_dev.bin");
  strcpy(g_cfg.ua.justin_cat_file,  "/opt/ab/justin_cat.csv");
  strcpy(g_cfg.ua.os_file,  "/opt/ab/os.csv");
  strcpy(g_cfg.ua.browser_file,  "/opt/ab/browser.csv");
  strcpy(g_cfg.ua.device_type_file,  "/opt/ab/device_type.csv");
  strcpy(g_cfg.ua.ua_model_coeff_file,  "/opt/ab/model_coefficients.csv");
  strcpy(g_cfg.ua.ua_category_intercept_file,  "/opt/ab/category_intercept.csv");
  // STOP : For user agent classifier

  // strcpy(g_cfg.dt_dir,  "../DT/spam/");
  // strcpy(g_cfg.ua_dir,  "../DT/spam/");

  strcpy(g_cfg.mmdb_file,  "/opt/ab/GeoIP2-City_2017_12_08.mmdb");

  // defaults taken from https://kafka.apache.org/08/documentation.html

  strcpy(g_cfg.kafka.brokers, "localhost");
  strcpy(g_cfg.kafka.topic, "ab");
  strcpy(g_cfg.kafka.retries, "4");
  strcpy(g_cfg.kafka.max_buffering_time, "5000");


  /*
     strcpy(g_cfg.kafka.brokers, "192.168.1.151");
     strcpy(g_cfg.kafka.topic, "ab");
     strcpy(g_cfg.kafka.queue_size, "1048576");
     strcpy(g_cfg.kafka.retries, "4");
     strcpy(g_cfg.kafka.max_buffering_time, "5000");
     */

}
