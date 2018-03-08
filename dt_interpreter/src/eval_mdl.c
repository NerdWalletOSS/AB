#include "dt_incs.h"
#include "eval_dt.h"
#include "eval_rf.h"
#include "eval_mdl.h"

int
eval_mdl(
  float *features, /* [n_features] */
  int n_features,
  DT_REC_TYPE *dt, // [n_dt] 
  int n_dt, /* number of nodes in decision tree */
  RF_REC_TYPE *rf, // [n_rf] 
  int n_rf, /* number of decision trees in random forest */
  MDL_REC_TYPE *mdl,
  int n_mdl /* number of models */
  )
{
  int status = 0;
  for ( int i = 0; i < n_mdl; i++ ) {
    mdl[i].prob = 0;
  }
#pragma omp parallel for 
  for ( int i = 0; i < n_mdl; i++ ) {
    int l_status = 0;
    if ( l_status < 0 ) { status = -1; continue; }
    int rf_lb = mdl[i].rf_lb;
    int rf_ub = mdl[i].rf_ub;
    int this_n_rf = rf_ub - rf_lb; // TODO Confirm no +1
    l_status = eval_rf(features, n_features, dt, n_dt, rf+rf_lb, this_n_rf);
    if ( l_status < 0 ) { status = -1; continue; }
    // Convert array of npos/nneg to prob
    double sum_prob = 0;
    for ( int j = rf_lb; j < rf_ub; j++ ) { 
      int npos = rf[j].npos;
      int nneg = rf[j].nneg;
      float prob = (double)npos/(double)(npos+nneg);
      sum_prob += prob;
    }
    mdl[i].prob = sum_prob / n_rf;
  }
  cBYE(status);
BYE:
  return status;
}
