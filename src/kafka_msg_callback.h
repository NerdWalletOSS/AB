#include "ab_incs.h"
#include "rdkafka.h"

void kafka_msg_callback (rd_kafka_t *g_rk,
    const rd_kafka_message_t *rkmessage, void *opaque);

