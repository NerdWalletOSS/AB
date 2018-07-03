extern int STATSD_INC(const char *stat);
extern int STATSD_DEC(const char *stat);
extern int STATSD_COUNT(const char *stat, size_t cnt);
extern int STATSD_GAUGE(const char *stat, size_t value);
extern int STATSD_TIMING(const char *stat, size_t ms);
