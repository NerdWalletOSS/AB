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
  int n_rf,
  int rf_lb,
  int rf_ub,
  // rf_eval is results of evaluating random forest
  RF_EVAL_REC_TYPE *rf_eval /* [n_rf] */ 
  )
{
  int status = 0;
  if ( dt == NULL ) { go_BYE(-1); }
  if ( n_dt == 0 ) { go_BYE(-1); }
  if ( rf == NULL ) { go_BYE(-1); }
  if ( n_rf <= 0 ) { go_BYE(-1); }
  if ( rf_lb < 0 ) { go_BYE(-1); }
  if ( rf_ub > n_rf ) { go_BYE(-1); }
  if ( rf_ub <= rf_lb ) { go_BYE(-1); }
  int idx = 0;
  for ( int i = rf_lb; i < rf_ub; i++, idx++ ) {
    int dt_lb = rf[i].dt_lb;
    int dt_ub = rf[i].dt_ub;
    status = eval_dt(features, n_features, dt, n_dt, dt_lb, dt_ub,
        &(rf_eval[idx]).npos, &(rf_eval[idx]).nneg, &(rf_eval[idx].xgb));
    cBYE(status);
  }
BYE:
  return status;
}
