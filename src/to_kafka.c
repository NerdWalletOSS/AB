#include "ab_incs.h"
#include "ab_globals.h"
#include "log_decision.h"
#include "to_kafka.h"
int
to_kafka(
    char *body,
    size_t sz_body
    )
{
  int status = 0;
  if ( ( body == NULL ) || ( sz_body == 0 ) ) { go_BYE(-1); }
  KAFKA_REC_TYPE X;

  g_log_kafka_calls++;
  X.sz = sz_body + 1;
  g_kafka_memory += X.sz;
  X.data = (char *)malloc(X.sz);
  return_if_malloc_failed(X.data);
  strcpy(X.out_tracer, g_out_tracer);
  memcpy(X.data, body, X.sz);
  // printf("Allocated %8x \n", (unsigned int)x);
  status = log_decision(&X); cBYE(status);
BYE:
  return status;
}
