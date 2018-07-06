#include "ab_incs.h"
#include "ab_constants.h"
#include "auxil.h"
#include "validate_config.h"

extern char g_valid_chars_in_url[256]; 
extern bool g_disable_dt; // set to false if no decision tree
extern bool g_disable_ua; // set to false if no user agent classifier
extern bool g_disable_ip; // set to false if no MaxMind Database
extern bool g_disable_sd; // set to false if no statsd server
extern bool g_disable_wa; // disable WebApp
extern bool g_disable_lg; // disable Logger
extern bool g_disable_kf; // disable Kafka

static bool
chk_url(
    const char *X
    )
{
  if ( *X == '\0' ) { return false; }
  for ( const char *cptr = X; *cptr != '\0'; cptr++ ) { 
    if ( ( !isalnum(*cptr) )  && 
          ( *cptr != '/' )  &&
          ( *cptr != '_' ) ) {
      return false;
    }
  }
  return true;
}


static bool
chk_server_name(
    const char *X
    )
{
  if ( *X == '\0' ) { return false; }
  for ( const char *cptr = X; *cptr != '\0'; cptr++ ) { 
    if ( !isascii(*cptr) ) { return false; }
  }
  return true;
}

int
validate_config(
    CFG_TYPE cfg
    )
{
  int status = 0;
  char *buf = NULL;
  int32_t ival;

  if ( cfg.sz_log_q <= 1024 ) { go_BYE(-1); }
  if ( ( cfg.port <= 0 ) ||  ( cfg.port >= 65536 ) ) { go_BYE(-1); }
  if ( cfg.max_len_uuid <= 0 ) { go_BYE(-1); }
  if ( cfg.num_post_retries < 0 ) { cfg.num_post_retries = 0; }
  //---------------------------------
  if ( cfg.default_url[0] == '\0' ) { go_BYE(-1); }
  if ( ( strncmp(cfg.default_url, "https://", 8) != 0 ) &&
      ( strncmp(cfg.default_url, "http://", 7) != 0 ) ) {
    go_BYE(-1); 
  }
  for ( char *cptr = cfg.default_url; *cptr != '\0'; cptr++ ) { 
    if ( !g_valid_chars_in_url[(uint8_t)(*cptr)] ) { go_BYE(-1); }
  }
  //---------------------------------
  if ( !g_disable_ua ) { 
    if ( !isdir(cfg.dt_dir) ) { go_BYE(-1); }
    if ( cfg.model_name[0] == '\0' ) { go_BYE(-1); }
    int len = strlen(cfg.dt_dir) + strlen(cfg.model_name) + 8;
    buf = calloc(len, 1);
    sprintf(buf, "%s/%s", cfg.dt_dir, cfg.model_name);
    if ( !isdir(buf) ) { go_BYE(-1); }
  }
  else {
    if ( cfg.model_name[0] != '\0' ) { go_BYE(-1); }

  }
  //---------------------------------
  if ( !g_disable_ip ) { 
    if ( !isfile(cfg.mmdb_file) ) { go_BYE(-1); }
  }
  //---------------------------------
  if ( !g_disable_sd ) { 
    if ( cfg.statsd_keys.inc[0]    == '\0' ) { go_BYE(-1); }
    if ( cfg.statsd_keys.count[0]  == '\0' ) { go_BYE(-1); }
    if ( cfg.statsd_keys.gauge[0]  == '\0' ) { go_BYE(-1); }
    if ( cfg.statsd_keys.timing[0] == '\0' ) { go_BYE(-1); }

    if ( !chk_server_name(cfg.statsd.server) ) { go_BYE(-1); }
    if ( ( cfg.statsd.port <= 0 ) ||  
         ( cfg.statsd.port >= 65536 ) ) { go_BYE(-1); }
  }
  //---------------------------------
  if ( !g_disable_lg ) { 
    if ( !chk_server_name(cfg.logger.server) ) { go_BYE(-1); }
    if ( !chk_url(cfg.logger.url) ) { go_BYE(-1); }
    if ( !chk_url(cfg.logger.health_url) ) { go_BYE(-1); }
    if ( ( cfg.logger.port <= 0 ) ||  
         ( cfg.logger.port >= 65536 ) ) { go_BYE(-1); }
  }
  //---------------------------------
  if ( !g_disable_wa ) { 
    if ( !chk_server_name(cfg.webapp.server) ) { go_BYE(-1); }
    if ( !chk_url(cfg.webapp.url) ) { go_BYE(-1); }
    if ( !chk_url(cfg.webapp.health_url) ) { go_BYE(-1); }
    if ( ( cfg.webapp.port <= 0 ) ||  
         ( cfg.webapp.port >= 65536 ) ) { go_BYE(-1); }
  }
  //---------------------------------
  if ( !g_disable_kf ) { 
    if ( cfg.kafka.brokers[0] == '\0' ) { go_BYE(-1); }
    if ( cfg.kafka.topic[0] == '\0' ) { go_BYE(-1); }
    status = stoI4(cfg.kafka.retries, &ival); 
    if ( ival < 0 ) { go_BYE(-1); }
    status = stoI4(cfg.kafka.max_buffering_time, &ival); 
    if ( ival < 0 ) { go_BYE(-1); }
  }
BYE:
  free_if_non_null(buf);
  return status;
}
