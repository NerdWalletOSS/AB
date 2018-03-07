
int
eval(
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
  int *ptr_npos = -1;
  int *ptr_ppos = -1;
  for ( ; ; ) { /* infinite loop */
    int fidx = dt[tidx].feature;
    double val       = in[fidx];
    double threshold = dt[tidx].threshold;

    if ( ( dt[tidx].left_child < 0 ) && ( dt[tidx].right_child < 0 ) ) {
      goto DONE;
    }
    if ( val <= threshold ) { 
      is_left = true; 
    }
    else {
      is_left = false;
    }
    if ( ( is_left )  && ( dt[tidx].left_child < 0 ) ) {
      goto DONE;
    }
    if ( ( !is_left )  && ( dt[tidx].right_child < 0 ) ) {
      goto DONE;
    }
    if ( is_left ) { 
      tidx = dt[tidx].left_child;
    }
    else {
      tidx = dt[tidx].right_child;
    }
  }
DONE:
  int *ptr_npos = dt[tidx].npos;
  int *ptr_nneg = dt[tidx].nneg;
BYE:
  return status;
}




  


