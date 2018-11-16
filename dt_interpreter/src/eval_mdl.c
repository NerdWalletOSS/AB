#include "dt_incs.h"
#include "eval_dt.h"
#include "eval_rf.h"
#include "eval_mdl.h"

#define RANDOM_FOREST 1
#define XGBOOST       2
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
  const char *const str_forest_type,
  float *predictions /* [n_mdl] */
  )
{
  int status = 0;
  for ( int i = 0; i < n_mdl; i++ ) {
    predictions[i] = 0;
  }
  int forest_type = -1;
  if ( strcasecmp(str_forest_type, "random_forest") == 0 ) { 
    forest_type = RANDOM_FOREST;
  }
  if ( strcasecmp(str_forest_type, "xgboost") == 0 ) { 
    forest_type = XGBOOST;
  }
  else {
    go_BYE(-1);
  }

  if ( dt == NULL ) { go_BYE(-1); }
  if ( n_dt == 0 ) { go_BYE(-1); }
  if ( rf == NULL ) { go_BYE(-1); }
  if ( n_rf == 0 ) { go_BYE(-1); }
  if ( mdl == NULL ) { go_BYE(-1); }
  if ( n_mdl == 0 ) { go_BYE(-1); }
  // TODO P1 Why does it work worse when we do not specify num_threads
  // Seems to me that system is using 8 threads 
  // (beacause of hyper-threading) but that makes matters worse
#pragma omp parallel for schedule(static, 1) num_threads(4)
  for ( int i = 0; i < n_mdl; i++ ) {
    int l_status = 0;
    if ( l_status < 0 ) { status = -1; continue; }
    int rf_lb = mdl[i].rf_lb;
    int rf_ub = mdl[i].rf_ub;
    int l_n_rf = rf_ub - rf_lb; 
    // Below: Avoiding malloc but introducing some ugliness
#define MAX_NUM_RF 1024 
    if ( l_n_rf > MAX_NUM_RF ) { l_status = -1; continue; }
    RF_EVAL_REC_TYPE rf_eval[MAX_NUM_RF];
    //-----------------------------
    l_status = eval_rf(features, n_features, dt, n_dt, 
        rf, n_rf, rf_lb, rf_ub, rf_eval);
    if ( l_status < 0 ) { status = -1; continue; }
    // Convert array of npos/nneg to prob
    double sum_prob = 0; 
    int npos;
    int nneg;
    float prob;
    float xgb;
    for ( int j = 0; j < l_n_rf; j++ ) { 
      switch ( forest_type ) { 
        case RANDOM_FOREST : 
          npos = rf_eval[j].npos;
          nneg = rf_eval[j].nneg;
          prob = (float)npos / (float)(npos + nneg);
          sum_prob += prob;
          break;
        case XGBOOST : 
          xgb = rf_eval[j].xgb;
          break;
      }
    }
    predictions[i] = sum_prob / l_n_rf;
  }
  cBYE(status);
BYE:
  return status;
}
