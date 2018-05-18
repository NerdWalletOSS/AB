#include <stdlib.h>
#include <string.h>
#include "ab_constants.h"
#include "ab_types.h"
#include "ab_globals.h"
#include "macros.h"
#include "auxil.h"
#include "dump_log.h"

void
write_log(
    char *buf,
    int *ptr_buf_idx,
    int buf_sz,
    const char *const label,
    uint64_t n
    )
{
  char temp[1024];
  sprintf(temp, " \"%s\" : %" PRIu64 ",", label, n);
  int len = strlen(temp);
  if ( ( *ptr_buf_idx + len ) <= buf_sz ) { 
    *ptr_buf_idx += len;
    strcat(buf, temp); 
  }
  else {
    WHEREAMI;
  }
}

int
dump_log(
    )
{
  int status = 0;
  int ridx = 0;

  strcpy(g_rslt, "{ ");
  ridx = strlen(g_rslt);

  //---- Scalars
  int n = AB_MAX_LEN_RESULT;
  write_log(g_rslt, &ridx, n, "StartTime", g_log_start_time);

  write_log(g_rslt, &ridx, n, "NoUserAgent",  g_log_no_user_agent);
  write_log(g_rslt, &ridx, n, "BadUserAgent", g_log_bad_user_agent);
  write_log(g_rslt, &ridx, n, "BadABArgs",    g_log_bad_ab_args);


  write_log(g_rslt, &ridx, n, "SessionServiceCalls", g_log_ss_calls);
  write_log(g_rslt, &ridx, n, "SessionServiceBadCode", g_log_ss_bad_code);
  write_log(g_rslt, &ridx, n, "SessionServiceTimeout", g_log_ss_timeout);
  write_log(g_rslt, &ridx, n, "SessionServiceNonAscii", g_log_ss_non_ascii);
  write_log(g_rslt, &ridx, n, "SessionServiceBadJSON", g_log_ss_bad_json);
  write_log(g_rslt, &ridx, n, "SessionServiceNoSession", g_log_ss_no_session);
  write_log(g_rslt, &ridx, n, "SessionServiceBadCall", g_log_ss_bad_calls);

  write_log(g_rslt, &ridx, n, "MissingTest",   g_log_missing_test);

  write_log(g_rslt, &ridx, n, "NoTestName",    g_log_no_test_name);
  write_log(g_rslt, &ridx, n, "NoTestNames",   g_log_no_test_names);
  write_log(g_rslt, &ridx, n, "NoUUID",        g_log_no_uuid);
  write_log(g_rslt, &ridx, n, "NoTestType",    g_log_no_test_type);
  write_log(g_rslt, &ridx, n, "NoTracer",      g_log_no_tracer);

  write_log(g_rslt, &ridx, n, "NumDroppedPosts", g_log_dropped_posts);
  write_log(g_rslt, &ridx, n, "NumPosts",        g_log_posts);
  write_log(g_rslt, &ridx, n, "BadPosts",        g_log_bad_posts);
  write_log(g_rslt, &ridx, n, "FailedPosts",     g_log_failed_posts);

  write_log(g_rslt, &ridx, n, "BadUUID",     g_log_bad_uuid);
  write_log(g_rslt, &ridx, n, "BadTestType", g_log_bad_test_type);
  write_log(g_rslt, &ridx, n, "BadTestName", g_log_bad_test_name);
  write_log(g_rslt, &ridx, n, "BadTracer",   g_log_bad_tracer);

  write_log(g_rslt, &ridx, n, "NumGetAltVariantCalls", g_log_get_alt_variant_calls);
  write_log(g_rslt, &ridx, n, "NumGetVariantCalls", g_log_get_variant_calls);
  write_log(g_rslt, &ridx, n, "NumGetVariantsCalls", g_log_get_variant_calls);
  write_log(g_rslt, &ridx, n, "NumRouterCalls",     g_log_router_calls);
  write_log(g_rslt, &ridx, n, "NumKafkaCalls",      g_log_kafka_calls);
  write_log(g_rslt, &ridx, n, "KafkaMemory",        g_kafka_memory);
  write_log(g_rslt, &ridx, n, "NumBadRouterCalls",  g_log_bad_router_calls);
  write_log(g_rslt, &ridx, n, "NumProbes",          g_log_num_probes);
  write_log(g_rslt, &ridx, n, "ResponseTime",       g_log_response_time);
  write_log(g_rslt, &ridx, n, "NumInQueue",         g_n_log_q);
  if ( strlen(g_rslt) > AB_MAX_LEN_RESULT - 32 ) { WHEREAMI; return -1; }
  strcat(g_rslt, "\"LastKey\" : 0 } \n");
  return status;
}

