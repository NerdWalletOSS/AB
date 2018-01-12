#include <stdlib.h>
#include <string.h>
#include "ab_constants.h"
#include "ab_types.h"
#include "ab_globals.h"
#include "macros.h"
#include "auxil.h"
#include "dump_log.h"

int
dump_log(
    const char * const args
    )
{
  int status = 0;
  char log_dir[255+1];
  FILE *fp = NULL;
  char *file_name = NULL;

  if ( g_num_log_entries == 0 ) { go_BYE(-1); }

  memset(log_dir, '\0', 255+1);
  status = extract_name_value(args, "LogDirectory=", '&', log_dir, 255);
  cBYE(status);
  if ( *log_dir == '\0' ) { go_BYE(-1); }
  int len = strlen(log_dir) + 64;
  file_name = malloc(len); return_if_malloc_failed(file_name);

  memset(file_name, '\0', len);
  strcpy(file_name, log_dir);
  strcat(file_name, "/_scalars.csv");
  fp = fopen(file_name, "w");
  return_if_fopen_failed(fp, file_name, "w");
  //---- Scalars
  fprintf(fp, "StartTime,%" PRIu64 "\n",     g_log_start_time);

  fprintf(fp, "RouterCalls,%" PRIu64 "\n",      g_log_router_calls);
  fprintf(fp, "NoUserAgent,%" PRIu64 "\n",     g_log_no_user_agent);
  fprintf(fp, "BadUserAgent,%" PRIu64 "\n",    g_log_bad_user_agent);

  fprintf(fp, "SessionServiceCalls,%" PRIu64 "\n", g_log_ss_calls);
  fprintf(fp, "SessionServiceNonAscii,%" PRIu64 "\n", g_log_ss_non_ascii);
  fprintf(fp, "SessionServiceBadCode,%" PRIu64 "\n", g_log_ss_bad_code);
  fprintf(fp, "SessionServiceNoSession,%" PRIu64 "\n", g_log_ss_no_session);
  fprintf(fp, "SessionServiceBadJSON,%" PRIu64 "\n", g_log_ss_bad_json);

  fprintf(fp, "MissingTestAB,%" PRIu64 "\n",   g_log_missing_test_ab);
  fprintf(fp, "MissingTestXY,%" PRIu64 "\n",   g_log_missing_test_xy);

  fprintf(fp, "NoTestName,%" PRIu64 "\n",      g_log_no_test_name);
  fprintf(fp, "NoTestNames,%" PRIu64 "\n",     g_log_no_test_names);

  fprintf(fp, "NumPosts,%" PRIu64 "\n",        g_log_posts);
  fprintf(fp, "BadPosts,%" PRIu64 "\n",        g_log_bad_posts);
  fprintf(fp, "FailedPosts,%" PRIu64 "\n",     g_log_failed_posts);

  fprintf(fp, "BadUUID,%" PRIu64 "\n",         g_log_bad_uuid);
  fprintf(fp, "BadTestType,%" PRIu64 "\n",     g_log_bad_test_type);
  fprintf(fp, "BadTestName%" PRIu64 "\n",      g_log_bad_test_name);

  fprintf(fp, "NumGetVariantCalls,%" PRIu64 "\n", g_num_get_variant_calls);
  fprintf(fp, "NumBadXYCalls,%" PRIu64 "\n",      g_num_bad_xy_calls);
  fprintf(fp, "NumPosts,%" PRIu64 "\n",           g_num_posts);

  fclose_if_non_null(fp);

BYE:
  free_if_non_null(file_name);
  fclose_if_non_null(fp);
  return status;
}

char *
log_common(
    uint64_t log_type,
    int *ptr_rec_size
    )
{
  if ( g_num_log_entries == 0 ) { return NULL; }
  uint64_t eff_idx = g_log_idx % g_num_log_entries;
  char *cptr = g_log + (eff_idx * AB_LOG_REC_SIZE);
  memset(cptr, '\0', AB_LOG_REC_SIZE);
  uint64_t *tptr = (uint64_t *)cptr;
  int rec_size = AB_LOG_REC_SIZE;
  rec_size -= sizeof(uint64_t); // to enforce space between records
  *tptr++ = g_log_time; rec_size -= sizeof(uint64_t);
  *tptr++ = log_type;   rec_size -= sizeof(uint64_t);
  *ptr_rec_size = rec_size;
  return (char *)tptr;
}

void
log_bad_user_agent(
    const char * const user_agent
    )
{
  int rec_size;
  if ( g_num_log_entries == 0 ) { return ; }
  char * cptr = log_common(AB_LOG_BAD_USER_AGENT, &rec_size);
  if ( cptr == NULL ) { WHEREAMI; return; }
  strncpy(cptr, user_agent, rec_size);
  g_log_bad_user_agent++;
  g_log_idx++;
}

void
log_missing_test(
    const char * const test_name,
    uint64_t test_type,
    uint64_t external_id
    )
{
  int rec_size;
  if ( g_num_log_entries == 0 ) { return ; }
  char * cptr = log_common(AB_LOG_MISSING_TEST, &rec_size);
  if ( cptr == NULL ) { WHEREAMI; return; }
  uint64_t *uptr = (uint64_t *)cptr;
  *uptr++ = test_type;   
  rec_size -= sizeof(uint64_t); if ( rec_size <= 0 ) { return; }

  *uptr++ = external_id; 
  rec_size -= sizeof(uint64_t); if ( rec_size <= 0 ) { return; }

  cptr = (char *)uptr;
  strncpy(cptr, test_name, rec_size);
  switch ( test_type ) { 
    case AB_TEST_TYPE_AB : g_log_missing_test_ab++; break;
    case AB_TEST_TYPE_XY : g_log_missing_test_xy++; break;
    default : WHEREAMI; break; 
  }
  g_log_idx++;
}

void
log_ss_calls(
    const char * const uuid,
    uint64_t response_time,
    uint64_t http_code,
    uint64_t is_null_data,
    uint64_t is_bad_data,
    const char *const ss_response
    )
{
  int rec_size;
  if ( g_num_log_entries == 0 ) { return ; }
  char * cptr = log_common(AB_LOG_SS_CALL, &rec_size);
  if ( cptr == NULL ) { WHEREAMI; return; }
  uint64_t *uptr = (uint64_t *)cptr;
  *uptr++ = response_time; 
  rec_size -= sizeof(uint64_t); if ( rec_size <= 0 ) { return; }

  *uptr++ = http_code;     
  rec_size -= sizeof(uint64_t); if ( rec_size <= 0 ) { return; }

  *uptr++ = is_null_data;  
  rec_size -= sizeof(uint64_t); if ( rec_size <= 0 ) { return; }

  *uptr++ = is_bad_data;   
  rec_size -= sizeof(uint64_t); if ( rec_size <= 0 ) { return; }

  cptr = (char *)uptr;
  int len = strlen(uuid);
  strncpy(cptr, uuid, rec_size);
  rec_size -= len; if ( rec_size <= 0 ) { return; }

  rec_size --; cptr++; // make space for nullc 
  if ( rec_size <= 0 ) { return; }

  strncpy(cptr, ss_response, rec_size);
  g_log_ss_calls++;
  g_log_idx++;
}
