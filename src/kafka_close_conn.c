#include "kafka_close_conn.h"

void 
kafka_close_conn(
    void
    ) 
{
  /* Wait for final messages to be delivered or fail.
   * rd_kafka_flush() is an abstraction over rd_kafka_poll() which
   * waits for all messages to be delivered. */
  if ( g_rk != NULL) {
    fprintf(stderr, "%% Flushing final messages..\n");
    rd_kafka_flush(g_rk, 1*1000 /* wait for max 1 second */);

    /* Destroy topic object */
    rd_kafka_topic_destroy(g_rkt);

    /* Destroy the producer instance */
    rd_kafka_destroy(g_rk);
    g_rk = NULL;
    g_rkt = NULL;
    g_kafka_conf = NULL;
  }
}
