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
    status = statsd_dec(g_statsd_link, stat, 1.0);
  }
  return status;
}


inline int STATSD_COUNT(char *stat, size_t cnt){
  int status = 0;
  if (g_statsd_link != NULL ) {
    status = statsd_count(g_statsd_link, stat, cnt, 1.0);
  }
  return status;
}

inline int STATSD_GAUGE(char *stat, size_t value){
  int status = 0;
  if (g_statsd_link != NULL ) {
    status = statsd_gauge(g_statsd_link, stat, value);
  }
  return status;
}

inline int STATSD_TIMING(char *stat, size_t ms){
  int status = 0;
  if (g_statsd_link != NULL ) {
    status = statsd_timing(g_statsd_link, stat, ms);
  }
  return status;
}
