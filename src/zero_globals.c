#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "ab_constants.h"
#include "ab_types.h"
#include "ab_globals.h"
#include "macros.h"
#include "zero_globals.h"
#include "auxil.h"
int 
zero_globals(
    void
    )
{
  int status = 0;
  //------------------------------
  g_ss_response = NULL;
  g_sz_ss_response = 0;

  g_statsd_link = NULL;

  g_n_log_q       = 0;
  g_log_q         = NULL;
  g_q_rd_idx      = 0;
  g_q_wr_idx      = 0;
  //------------------------------
  g_uuid          = NULL;

  memset(g_my_name, '\0', AB_MAX_LEN_HOSTNAME+1);
  status = gethostname(g_my_name, AB_MAX_LEN_HOSTNAME);
  cBYE(status);

BYE:
  return status;
}
