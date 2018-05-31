#include "dt_incs.h"
#include "eval_dt.h"
#include "eval_rf.h"
#include "eval_mdl.h"
extern uint64_t g_num_models;

int
eval_mdl(
  float *features, /* [n_features] */
  int n_features,
  DT_REC_TYPE *dt, // [n_dt] 
  int n_dt, /* number of nodes in decision tree */
  RF_REC_TYPE *rf, // [n_rf] 
  int n_rf, /* number of decision trees in random forest */
  MDL_REC_TYPE *mdl, /* [n_mdl] */
  int n_mdl, /* number of models */
  float *predictions, /* [n_mdl] */
  int *rf_pos, /* [n_rf] */
  int *rf_neg /* [n_rf] */
  )
{
  int status = 0;
  for ( int i = 0; i < n_mdl; i++ ) {
    predictions[i] = 0;
  }
  if ( dt == NULL ) { go_BYE(-1); }
  if ( n_dt == 0 ) { go_BYE(-1); }
  if ( rf == NULL ) { go_BYE(-1); }
  if ( n_rf == 0 ) { go_BYE(-1); }
  if ( mdl == NULL ) { go_BYE(-1); }
  if ( n_mdl == 0 ) { go_BYE(-1); }
#pragma omp parallel for schedule(static, 1) num_threads(4)
  for ( int i = 0; i < n_mdl; i++ ) {
    int l_status = 0;
    if ( l_status < 0 ) { status = -1; continue; }
    int rf_lb = mdl[i].rf_lb;
    int rf_ub = mdl[i].rf_ub;
    int this_n_rf = rf_ub - rf_lb; 
    l_status = eval_rf(features, n_features, dt, n_dt, 
        rf+rf_lb, this_n_rf, rf_pos+rf_lb, rf_neg+rf_lb);
    if ( l_status < 0 ) { status = -1; continue; }
    // Convert array of npos/nneg to prob
    double sum_prob = 0;
    for ( int j = rf_lb; j < rf_ub; j++ ) { 
      int npos = rf_pos[j];
      int nneg = rf_neg[j];
      float prob = (double)npos/(double)(npos+nneg);
      sum_prob += prob;
    }
    predictions[i] = sum_prob / n_rf;
  }
  cBYE(status);
BYE:
  return status;
}
