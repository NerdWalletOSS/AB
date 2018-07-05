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

  // strcpy(g_cfg.statsd.server, "localhost");
  // g_cfg.statsd.port = 8125;

  g_cfg.sz_log_q = 131072;
  g_cfg.num_post_retries = 1;

  strcpy(g_cfg.default_url, "http://localhost:8080/AB/test_webapp/index0.html");
  g_cfg.max_len_uuid = 47;

  // TODO: P1: Indrajeet to check key values below
  strcpy(g_cfg.statsd_keys.inc,   "nw.metrics.ab.requests");
  strcpy(g_cfg.statsd_keys.count, "nw.metrics.ab.counts");
  strcpy(g_cfg.statsd_keys.timing,"nw.metrics.ab.request_time");
  strcpy(g_cfg.statsd_keys.gauge, "nw.metrics.ab.gauge");

  strcpy(g_cfg.ua_dir,  "../USER_AGENT_CLASSIFIER/data/");
  strcpy(g_cfg.dt_dir,  "../DT/");
  strcpy(g_cfg.model_name,  "cc_member_models");
  strcpy(g_cfg.mmdb_file,  "/opt/ab/GeoIP2-City_2017_12_08.mmdb");

  // defaults taken from https://kafka.apache.org/08/documentation.html

    /*
  strcpy(g_cfg.kafka.brokers, "localhost");
  strcpy(g_cfg.kafka.topic, "ab");
  strcpy(g_cfg.kafka.retries, "4");

     strcpy(g_cfg.kafka.brokers, "192.168.1.151");
     strcpy(g_cfg.kafka.topic, "ab");
     strcpy(g_cfg.kafka.queue_size, "1048576");
     strcpy(g_cfg.kafka.retries, "4");
     strcpy(g_cfg.kafka.max_buffering_time, "5000");
     */

}
