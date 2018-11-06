#include "dt_incs.h"
#include "dt_types.h"
#include "dt_globals.h"
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
  int sz = 1024;
  char temp[sz];
  snprintf(temp, sz, " \"%s\" : %" PRIu64 ",", label, n);
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
  int n = DT_MAX_LEN_RESULT;

  write_log(g_rslt, &ridx, n, "StartTime", g_log_start_time);

  write_log(g_rslt, &ridx, n, "ResponseTime", g_log_response_time);
  write_log(g_rslt, &ridx, n, "NumClassify",  g_log_num_classify);

  strcat(g_rslt, "\"LastKey\" : 0 } \n");
  return status;
}
