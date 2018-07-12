#include "kafka_check_conn.h"


int kafka_check_conn(
    char* buf,
    size_t len
    )
{
  int status = 0;
  g_kafka_callback = 1 ;
  status = kafka_add_to_queue(buf, len);
  cBYE(status);
  rd_kafka_flush(g_rk, -1);
  rd_kafka_poll(g_rk, 1*1000); // wait for buffer to flush
  if (g_kafka_callback != 0) {
    g_kafka_callback = 0;
    go_BYE(-1);
  } 
BYE:
  return status;
}
