extern int
eval_dt(
  float *in, /* [n_features] */
  int n_features,
  DT_REC_TYPE *dt,
  int dt_lb,
  int dt_ub,
  int root_idx,
  int *ptr_npos, // return values
  int *ptr_nneg, // return values
  float *ptr_xgb // return values
  );
