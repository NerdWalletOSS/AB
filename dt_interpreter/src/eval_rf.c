#include "dt_incs.h"
#include "eval_dt.h"
#include "eval_rf.h"

int
eval_rf(
  float *features, /* [n_features] */
  int n_features,
  DT_REC_TYPE *dt, // [n_dt] 
  int n_dt, /* number of nodes in decision tree */
  RF_REC_TYPE *rf, // [n_rf] 
  int n_rf /* number of nodes in decision tree */
  )
{
  int status = 0;
  for ( int i = 0; i < n_rf; i++ ) {
    rf[i].npos = rf[i].nneg = -1;
  }
  for ( int i = 0; i < n_rf; i++ ) {
    int dt_lb = rf[i].dt_lb;
    int dt_ub = rf[i].dt_ub;
    status = eval_dt(features, n_features, dt+dt_lb, dt_ub-dt_lb+1,
        &(rf[i].npos), &(rf[i].nneg));
    cBYE(status);
  }
BYE:
  return status;
}
