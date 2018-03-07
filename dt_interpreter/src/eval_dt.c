#include "dt_incs.h"
#include "eval_dt.h"

int
eval_dt(
  double *in, /* [n_features] */
  int n_features,
  DT_REC_TYPE *dt,
  int n_dt, /* number of nodes in decision tree */
  int *ptr_npos, // return values
  int *ptr_nneg // return values
  )
{
  int status = 0;

  int tidx = 0; // start at root 
  *ptr_npos = -1; // set to something clearly wrong
  *ptr_npos = -1; // set to something clearly wrong
  for ( ; ; ) { /* infinite loop */
    bool is_left;
    int fidx = dt[tidx].feature_idx;
    double val       = in[fidx];
    double threshold = dt[tidx].threshold;

    if ( ( dt[tidx].lchild_idx < 0 ) && ( dt[tidx].rchild_idx < 0 ) ) {
      goto DONE;
    }
    if ( val <= threshold ) { 
      is_left = true; 
    }
    else {
      is_left = false;
    }
    if ( ( is_left )  && ( dt[tidx].lchild_idx < 0 ) ) {
      goto DONE;
    }
    if ( ( !is_left )  && ( dt[tidx].rchild_idx < 0 ) ) {
      goto DONE;
    }
    if ( is_left ) { 
      tidx = dt[tidx].lchild_idx;
    }
    else {
      tidx = dt[tidx].rchild_idx;
    }
  }
DONE:
  *ptr_npos = dt[tidx].npos;
  *ptr_nneg = dt[tidx].nneg;
  if ( ( *ptr_npos < 0 ) || ( *ptr_nneg < 0 ) ) { go_BYE(-1); }
BYE:
  return status;
}




  


