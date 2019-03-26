extern int
read_random_forest(
  const char *const file_name,
  int forest_type,
  DT_REC_TYPE **ptr_dt,
  int *ptr_n_dt, /* number of nodes in decision tree */
  RF_REC_TYPE **ptr_rf,
  int *ptr_n_rf, /* number of nodes in random forest */
  MDL_REC_TYPE **ptr_mdl,
  int *ptr_n_mdl /* number of models */
  );
