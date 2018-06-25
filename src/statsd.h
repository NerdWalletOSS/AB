#include "ab_globals.h"

inline int STATSD_INC(char *stat){
  int status = 0;
  if (g_statsd_link != NULL ) {
    status = statsd_inc(g_statsd_link, stat, 1.0);
  }
  return status;
}

inline int STATSD_DEC(char *stat){
  int status = 0;
  if (g_statsd_link != NULL ) {
    snprintf(g_statsd_buf, AB_MAX_LEN_REDIRECT_URL, "%s.%s", g_cfg.statsd_inc, stat);
    status = statsd_dec(g_statsd_link, g_statsd_buf, 1.0);
  }
  return status;
}


inline int STATSD_COUNT(char *stat, size_t cnt){
  int status = 0;
  if (g_statsd_link != NULL ) {
    snprintf(g_statsd_buf, AB_MAX_LEN_REDIRECT_URL, "%s.%s", g_cfg.statsd_count, stat);
    status = statsd_count(g_statsd_link, g_statsd_buf, cnt, 1.0);
  }
  return status;
}

inline int STATSD_GAUGE(char *stat, size_t value){
  int status = 0;
  if (g_statsd_link != NULL ) {
    snprintf(g_statsd_buf, AB_MAX_LEN_REDIRECT_URL, "%s.%s", g_cfg.statsd_gauge, stat);
    status = statsd_gauge(g_statsd_link, g_statsd_buf, value);
  }
  return status;
}

inline int STATSD_TIMING(char *stat, size_t ms){
  int status = 0;
  if (g_statsd_link != NULL ) {
   snprintf(g_statsd_buf, AB_MAX_LEN_REDIRECT_URL, "%s.%s", g_cfg.statsd_inc, stat);
   status = statsd_timing(g_statsd_link, g_statsd_buf, ms);
  }
  return status;
}
