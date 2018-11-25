extern int
eval_mdl(
  float *features, /* [n_features] */
  int n_features,
  DT_REC_TYPE *dt, // [n_dt] 
  int n_dt, /* number of nodes in decision tree */
  RF_REC_TYPE *rf, // [n_rf] 
  int n_rf, /* number of decision trees in random forest */
  MDL_REC_TYPE *mdl, /* [n_mdl] */
  int n_mdl, /* number of models */
  int forest_type,
  float *predictions /* [n_mdl] */
  );
