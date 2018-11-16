extern int
eval_rf(
  float *features, /* [n_features] */
  int n_features,
  DT_REC_TYPE *dt, // [n_dt] 
  int n_dt, /* number of nodes in decision tree */
  RF_REC_TYPE *rf, // [n_rf] 
  int n_rf, /* number of decision trees in *ALL* random forests */
  int rf_lb, /* first decision tree for this model */
  int rf_ub, /* last decision tree for this model */
  RF_EVAL_REC_TYPE *rf_eval /* [n_rf] */
  );
