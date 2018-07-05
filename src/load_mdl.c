#include "ab_incs.h"
#include "mmap.h"
#include "load_mdl.h"

int
load_mdl(
    char *mdl_file,
    MDL_REC_TYPE **ptr_mdl,
    size_t *ptr_len_file,
    uint32_t *ptr_num_mdl
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  status = rs_mmap(mdl_file, &X, &nX, 0); cBYE(status);
  if ( nX == 0 ) { go_BYE(-1); }
  uint32_t num_mdl = nX / sizeof(MDL_REC_TYPE);
  if ( ( num_mdl * sizeof(MDL_REC_TYPE) ) != nX ) { go_BYE(-1); }
  *ptr_mdl = (MDL_REC_TYPE *)X;
  *ptr_len_file = nX;
  *ptr_num_mdl = num_mdl;
BYE:
  return status;
}
