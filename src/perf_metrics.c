#include <stdio.h>
#include <stdbool.h>
#include "ab_constants.h"
#include "ab_types.h"
#include "macros.h"
#include "ab_globals.h"
#include "auxil.h"
#include "perf_metrics.h"

//<hdr>
int
perf_metrics(
    char *X,
    size_t nX 
    )
//</hdr>
{
  int status = 0;

  memset(X, '\0', nX);
  size_t sz = snprintf(X, nX,
      "{ \"StartTime\" : \"%llu\", \
      \"QueueSize\" : \"%" PRIu32 "\" }", g_t_start, g_n_log_q);
  if ( sz >= nX ) { go_BYE(-1); }
BYE:
  return status;
}
