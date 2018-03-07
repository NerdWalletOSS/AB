extern int
eval_dt(
  float *in, /* [n_features] */
  int n_features,
  DT_REC_TYPE *dt,
  int n_dt, /* number of nodes in decision tree */
  int *ptr_npos, // return values
  int *ptr_nneg // return values
  );
