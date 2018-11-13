#ifndef __DT_TYPES_H
#define __DT_TYPES_H

typedef struct _dt_rec_type { 
  int model_idx;
  int tree_idx;
  int node_idx;
  int lchild_idx;
  int rchild_idx;
  int feature_idx;
  float threshold;
  int nneg;
  int npos;
} DT_REC_TYPE;

typedef struct _rf_rec_type { 
  int tree_idx;
  int dt_lb; // lower bound (inclusive) into dt structure
  int dt_ub; // upper bound (exclusive) into dt structure
  int nneg; // these will get set at run time 
  int npos; // these will get set at run time 
  int model_idx;
} RF_REC_TYPE;

typedef struct _mdl_rec_type { 
  int model_idx;
  int rf_lb; // lower bound (inclusive) into rf structure
  int rf_ub; // upper bound (exclusive) into rf structure
  float prob; // will get set at run time 
} MDL_REC_TYPE;

typedef struct _dt_interpreter_type {
  DT_REC_TYPE *dt; /* decision tree [n_dt]  */
  uint32_t n_dt;
  size_t len_dt_file; 
  RF_REC_TYPE *rf; /* random forest * [n_rf] */
  uint32_t n_rf;
  size_t len_rf_file; 
  MDL_REC_TYPE *mdl; /* models [n_mdl] */
  uint32_t n_mdl;
  size_t len_mdl_file; 

  float *predictions;  /* [n_mdl] */
  float *dt_feature_vector; 
  int n_dt_feature_vector  ;  // Set by Lua after reading configs
} DT_INTERPRETER_TYPE;
#endif
