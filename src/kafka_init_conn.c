#include "kafka_init_conn.h"

int kafka_init_conn(const char* topic, const char* brokers) {
  /*
   * Create Kafka client configuration place-holder
   */
  if (g_kafka_conf != NULL) {
    kafka_close_conn();
  }
  g_kafka_conf = rd_kafka_conf_new();

  /* Set bootstrap broker(s) as a comma-separated list of
   * host or host:port (default port 9092).
   * librdkafka will use the bootstrap brokers to acquire the full
   * set of brokers from the cluster. */
  if (rd_kafka_conf_set(g_kafka_conf, "bootstrap.servers", brokers,
        g_errstr, sizeof(g_errstr)) != RD_KAFKA_CONF_OK) {
    fprintf(stderr, "%s\n", g_errstr);
    // add error to g_err
    return 1;
  }

  const char* queue_size = "1000000"; // max 10000000
  /* Set message queue size */
  if (rd_kafka_conf_set(g_kafka_conf, "queue.buffering.max.messages", queue_size,
        g_errstr, sizeof(g_errstr)) != RD_KAFKA_CONF_OK) {
    fprintf(stderr, "%s\n", g_errstr);
    // add error to g_err
    return 1;
  }

  /* Set max retries before returning an error */
  const char* retries = "5"; // max 10000000
  /* Set message queue size */
  if (rd_kafka_conf_set(g_kafka_conf, "message.send.max.retries", retries,
        g_errstr, sizeof(g_errstr)) != RD_KAFKA_CONF_OK) {
    fprintf(stderr, "%s\n", g_errstr);
    // add error to g_err
    return 1;
  }



  /* Set the delivery report callback.
   * This callback will be called once per message to inform
   * the application if delivery succeeded or failed.
   * See kafka_msg_callback() above. */
  rd_kafka_conf_set_dr_msg_cb(g_kafka_conf, kafka_msg_callback);


  /*
   * Create producer instance.
   *
   * NOTE: rd_kafka_new() takes ownership of the conf object
   *       and the application must not reference it again after
   *       this call.
   */
  g_rk = rd_kafka_new(RD_KAFKA_PRODUCER, g_kafka_conf, g_errstr, sizeof(g_errstr));
  if (!g_rk) {
    fprintf(stderr,
        "%% Failed to create new producer: %s\n", g_errstr);
    // add err to g_err
    return 1;
  }


  /* Create topic object that will be reused for each message
   * produced.
   *
   * Both the producer instance (rd_kafka_t) and topic objects (topic_t)
   * are long-lived objects that should be reused as much as possible.
   */
  g_rkt = rd_kafka_topic_new(g_rk, topic, NULL);
  if (!g_rkt) {
    fprintf(stderr, "%% Failed to create topic object: %s\n",
        rd_kafka_err2str(rd_kafka_last_error()));
    rd_kafka_destroy(g_rk);
    // add to g_err
    return 1;
  }
  return 0;
}


