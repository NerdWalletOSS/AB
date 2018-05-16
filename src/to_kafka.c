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

  void *x = malloc(sz_body);
  return_if_malloc_failed(x);

  status = log_decision(x); cBYE(status);
BYE:
  return status;
}
