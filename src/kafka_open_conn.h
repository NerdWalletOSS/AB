#include "ab_incs.h"
#include "ab_globals.h"
#include "kafka_close_conn.h"
#include "kafka_msg_callback.h"
#include "rdkafka.h"

int kafka_open_conn(const char* topic, const char* brokers);
