#include "dt_incs.h"
#include "read_random_forest.h"

int
read_random_forest(
  const char *const file_name,
  DT_REC_TYPE **ptr_dt,
  int *ptr_n_dt, /* number of nodes in decision tree */
  RF_REC_TYPE **ptr_rf,
  int *ptr_n_rf /* number of nodes in random forest */
  )
{
  int status = 0;
  DT_REC_TYPE *dt = NULL;
  int n_dt = 0;
  // count number of lines 
  status = rs_mmap(&X, &nX, file_name, 0);  cBYE(status);
  if ( ( X == NULL ) || ( nX == 0 ) ) { go_BYE(-1); }
BYE:
  return status;
}
