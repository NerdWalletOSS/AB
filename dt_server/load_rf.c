#include "dt_incs.h"
#include "mmap.h"
#include "load_rf.h"

int
load_rf(
    char *rf_file,
    RF_REC_TYPE **ptr_rf,
    size_t *ptr_len_file,
    uint32_t *ptr_num_rf
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  status = rs_mmap(rf_file, &X, &nX, 0); cBYE(status);
  if ( nX == 0 ) { go_BYE(-1); }
  uint32_t num_rf = nX / sizeof(RF_REC_TYPE);
  if ( ( num_rf * sizeof(RF_REC_TYPE) ) != nX ) { go_BYE(-1); }
  *ptr_rf = (RF_REC_TYPE *)X;
  *ptr_len_file = nX;
  *ptr_num_rf = num_rf;
BYE:
  return status;
}
