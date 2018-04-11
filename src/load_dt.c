#include "ab_incs.h"
#include "mmap.h"
#include "load_dt.h"

int
load_dt(
    char *dt_file,
    DT_REC_TYPE **ptr_dt,
    size_t *ptr_len_file,
    uint32_t *ptr_num_dt
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  status = rs_mmap(dt_file, &X, &nX, 0); cBYE(status);
  if ( nX == 0 ) { go_BYE(-1); }
  uint32_t n_ua = nX / sizeof(DT_REC_TYPE);
  if ( ( n_ua * sizeof(DT_REC_TYPE) ) != nX ) { go_BYE(-1); }
  *ptr_dt = (DT_REC_TYPE *)X;
  *ptr_len_file = nX;
  *ptr_num_dt = n_ua;
BYE:
  return status;
}
