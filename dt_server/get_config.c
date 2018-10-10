#include "dt_incs.h"
#include "auxil.h"
#include "get_config.h"

extern char g_rslt[DT_MAX_LEN_RESULT+1]; // For C: ab_process_req()

int
get_config(
    void
    )
{
  int status = 0;
  strcpy(g_rslt, " { \"TODO\" : true } ");
BYE:
  return status;
}
