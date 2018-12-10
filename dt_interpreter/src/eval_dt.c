#include "dt_incs.h"
#include "eval_dt.h"

extern uint64_t g_num_compares;
int
eval_dt(
  float *features, /* [n_features] */
  int n_features,
  DT_REC_TYPE *dt,
  int n_dt,
  int dt_lb,
  int dt_ub,
  int *ptr_npos, // return values
  int *ptr_nneg, // return values
  float *ptr_xgb// return values
  )
{
  int status = 0;
  if ( dt == NULL ) { go_BYE(-1); }
  if ( n_dt <= 0 ) { go_BYE(-1); }
  if ( dt_lb < 0 ) { go_BYE(-1); }
  if ( dt_ub > n_dt ) { go_BYE(-1); }
  if ( dt_ub <= dt_lb ) { go_BYE(-1); }
  int tidx = dt_lb; // start at root 
  *ptr_npos = *ptr_nneg = -1; // some clearly bad value
  for ( ; ; ) { /* infinite loop */
    // g_num_compares++;
    // debugging
    if ( ( tidx < dt_lb ) || ( tidx >= dt_ub ) ) { go_BYE(-1); }
    if ( ( dt[tidx].lchild_idx < 0 ) && ( dt[tidx].rchild_idx < 0 ) ) {
      break;
    }
    bool is_left;
    int fidx = dt[tidx].feature_idx;
    // debugging
    if ( ( fidx < 0 ) || ( fidx >= n_features ) ) { go_BYE(-1); }
    float val       = features[fidx];
    float threshold = dt[tidx].threshold;

    if ( val <= threshold ) { 
      is_left = true; 
    }
    else {
      is_left = false;
    }
    if ( ( is_left )  && ( dt[tidx].lchild_idx < 0 ) ) {
      break;
    }
    if ( ( !is_left )  && ( dt[tidx].rchild_idx < 0 ) ) {
      break;
    }
    if ( is_left ) { 
      tidx = dt[tidx].lchild_idx;
    }
    else {
      tidx = dt[tidx].rchild_idx;
    }
  }
  *ptr_npos = dt[tidx].npos;
  *ptr_nneg = dt[tidx].nneg;
  // TODO: Any restrictions on xgb val?
  *ptr_xgb  = dt[tidx].xgb;
  if ( ( *ptr_npos < 0 ) || ( *ptr_nneg < 0 ) ) { go_BYE(-1); }
BYE:
  return status;
}
