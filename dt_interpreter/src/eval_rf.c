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
  int n_rf, /* number of nodes in decision tree */
  int *rf_pos, /* [n_rf] */
  int *rf_neg /* [n_rf] */
  )
{
  int status = 0;
  if ( dt == NULL ) { go_BYE(-1); }
  if ( n_dt == 0 ) { go_BYE(-1); }
  if ( rf == NULL ) { go_BYE(-1); }
  if ( n_rf == 0 ) { go_BYE(-1); }
  for ( int i = 0; i < n_rf; i++ ) {
    rf_pos[i] = rf_neg[i] = -1;
  }
  for ( int i = 0; i < n_rf; i++ ) {
    int dt_lb = rf[i].dt_lb;
    int dt_ub = rf[i].dt_ub;
    int l_n_dt = dt_ub - dt_lb;
    status = eval_dt(features, n_features, dt+dt_lb, 0, l_n_dt,
        &(rf_pos[i]), &(rf_neg[i]));
    cBYE(status);
  }
BYE:
  return status;
}
