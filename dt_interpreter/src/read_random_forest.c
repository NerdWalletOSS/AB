#include "dt_incs.h"
#include "mmap.h"
#include "read_random_forest.h"
#include "load_csv_fast.h"
#include "file_exists.h"

int
read_random_forest(
  const char *const file_name,
  DT_REC_TYPE **ptr_dt,
  int *ptr_n_dt, /* number of nodes in decision tree */
  RF_REC_TYPE **ptr_rf,
  int *ptr_n_rf, /* number of nodes in random forest */
  MDL_REC_TYPE **ptr_mdl,
  int *ptr_n_mdl /* number of models */
  )
{
  int status = 0;
  *ptr_dt = NULL;
  *ptr_n_dt = 0;
  *ptr_rf = NULL;
  *ptr_n_rf = 0;
  DT_REC_TYPE *dt = NULL; int n_dt = 0;
  RF_REC_TYPE *rf = NULL; int n_rf = 0;
  MDL_REC_TYPE *mdl = NULL; int n_mdl = 0;
  char *X = NULL; size_t nX = 0;

  // tree_idx,node_idx,lchild_idx,rchild_idx,feature_idx,threshold,nnneg,npos
  const char *const q_data_dir = "/tmp/";
  const char *const infile = file_name;
  uint32_t nC = 10; // HARD CODED
  uint64_t nR = 0;
  char *fldtypes[nC];
  for ( uint32_t i = 0; i < nC; i++ ) { fldtypes[i] = NULL; }
  fldtypes[0] = strdup("I4");
  fldtypes[1] = strdup("I4");
  fldtypes[2] = strdup("I4");
  fldtypes[3] = strdup("I4");
  fldtypes[4] = strdup("I4");
  fldtypes[5] = strdup("I4");
  fldtypes[6] = strdup("F4");
  fldtypes[7] = strdup("I4"); 
  fldtypes[8] = strdup("I4"); 
  fldtypes[9] = strdup("F4"); 
  bool is_hdr = true;
  bool is_load[nC]; 
  for ( uint32_t i = 0; i < nC; i++ ) { is_load[i] = true; }
  bool has_nulls[nC]; 
  for ( uint32_t i = 0; i < nC; i++ ) { has_nulls[i] = false; }
  uint64_t num_nulls[nC]; 
  for ( uint32_t i = 0; i < nC; i++ ) { num_nulls[i] = 0; }
  char **out_files = NULL;
  char **nil_files = NULL;
  char *str_for_lua = NULL;
  size_t sz_str_for_lua = 0;
  int n_str_for_lua = 0;

  status = load_csv_fast( q_data_dir, infile, nC, &nR, fldtypes, is_hdr, 
      is_load, has_nulls, num_nulls, &out_files, &nil_files, 
      str_for_lua, sz_str_for_lua, &n_str_for_lua );
  cBYE(status);
  n_dt = nR;
  dt = malloc(n_dt * sizeof(DT_REC_TYPE));
  return_if_malloc_failed(dt);
  int *iptr; float *fptr;
  // transfer stuff from columns into rows. TODO P4 IMPROVE 
  status = rs_mmap(out_files[0], &X, &nX, 0); cBYE(status);
  iptr = (int *)X;
  for ( int i = 0; i < n_dt; i++ ) { dt[i].model_idx = iptr[i]; }
  mcr_rs_munmap(X, nX);
  //----------------------------------------------------
  status = rs_mmap(out_files[1], &X, &nX, 0); cBYE(status);
  iptr = (int *)X;
  for ( int i = 0; i < n_dt; i++ ) { dt[i].tree_idx = iptr[i]; }
  mcr_rs_munmap(X, nX);
  //----------------------------------------------------
  status = rs_mmap(out_files[2], &X, &nX, 0); cBYE(status);
  iptr = (int *)X;
  for ( int i = 0; i < n_dt; i++ ) { dt[i].node_idx = iptr[i]; }
  mcr_rs_munmap(X, nX);
  //----------------------------------------------------
  status = rs_mmap(out_files[3], &X, &nX, 0); cBYE(status);
  iptr = (int *)X;
  for ( int i = 0; i < n_dt; i++ ) { dt[i].lchild_idx = iptr[i]; }
  mcr_rs_munmap(X, nX);
  //----------------------------------------------------
  status = rs_mmap(out_files[4], &X, &nX, 0); cBYE(status);
  iptr = (int *)X;
  for ( int i = 0; i < n_dt; i++ ) { dt[i].rchild_idx = iptr[i]; }
  mcr_rs_munmap(X, nX);
  //----------------------------------------------------
  status = rs_mmap(out_files[5], &X, &nX, 0); cBYE(status);
  iptr = (int *)X;
  for ( int i = 0; i < n_dt; i++ ) { dt[i].feature_idx = iptr[i]; }
  mcr_rs_munmap(X, nX);
  //----------------------------------------------------
  status = rs_mmap(out_files[6], &X, &nX, 0); cBYE(status);
  fptr = (float *)X;
  for ( int i = 0; i < n_dt; i++ ) { dt[i].threshold = fptr[i]; }
  mcr_rs_munmap(X, nX);
  //----------------------------------------------------
  status = rs_mmap(out_files[7], &X, &nX, 0); cBYE(status);
  iptr = (int *)X;
  for ( int i = 0; i < n_dt; i++ ) { dt[i].nneg = iptr[i]; }
  mcr_rs_munmap(X, nX);
  //----------------------------------------------------
  status = rs_mmap(out_files[8], &X, &nX, 0); cBYE(status);
  iptr = (int *)X; 
  for ( int i = 0; i < n_dt; i++ ) { dt[i].npos = iptr[i]; }
  mcr_rs_munmap(X, nX);
  //----------------------------------------------------
  status = rs_mmap(out_files[9], &X, &nX, 0); cBYE(status);
  fptr = (float *)X; 
  for ( int i = 0; i < n_dt; i++ ) { dt[i].xgb_val = fptr[i]; }
  mcr_rs_munmap(X, nX);
  //----------------------------------------------------
  // calculate number of models, alloc and initialize
  n_mdl = 0;
  for ( int i = 0; i < n_dt; i++ ) { 
    n_mdl = max(n_mdl, dt[i].model_idx);
  }
  n_mdl++;
  mdl = malloc(n_mdl * sizeof(MDL_REC_TYPE));
  return_if_malloc_failed(mdl);
  memset(mdl, '\0', n_mdl * sizeof(MDL_REC_TYPE));
  //----------------------------------------------------
  //  Basic integrity testing
  int n_fixed = 0;
  for ( int i = 1; i < n_dt; i++ ) { 
    if ( dt[i].node_idx != i ) { 
      go_BYE(-1); }
    if ( dt[i].tree_idx < dt[i-1].tree_idx ) { go_BYE(-1); }
    if ( dt[i].tree_idx != dt[i-1].tree_idx ) { 
      if ( dt[i].tree_idx != dt[i-1].tree_idx+1 ) { 
        go_BYE(-1);
      }
    }
    if ( dt[i].model_idx < dt[i-1].model_idx ) { go_BYE(-1); }
    if ( dt[i].model_idx != dt[i-1].model_idx ) { 
      if ( dt[i].model_idx != dt[i-1].model_idx+1 ) { 
        go_BYE(-1);
      }
    }
    if ( dt[i].feature_idx < 0 ) { 
      if ( ( dt[i].lchild_idx ==  dt[i].rchild_idx ) && 
          ( dt[i].lchild_idx != -1 ) ) {
        dt[i].lchild_idx = dt[i].rchild_idx = -1;
        n_fixed++;
      }
    }
    if ( ( dt[i].lchild_idx ==  dt[i].rchild_idx ) && 
         ( dt[i].lchild_idx != -1 ) ) {
      go_BYE(-1); }
  }
  if ( n_fixed > 0 ) { 
    printf("Fixed %d \n", n_fixed); go_BYE(-1);
  }
  //----------------------------------------------------
  // calculate number of random forests
  n_rf = 0;
  for ( int i = 0; i < n_dt; i++ ) { 
    n_rf = max(n_rf, dt[i].tree_idx);
  }
  n_rf++;
  rf = malloc(n_rf * sizeof(RF_REC_TYPE));
  return_if_malloc_failed(rf);
  // TODO P4 Fairly dumb way to initialize rf .Can be improved
  for ( int i = 0; i < n_rf; i++ ) { 
    rf[i].tree_idx = i;
    rf[i].dt_lb = INT_MAX;
    rf[i].dt_ub = INT_MIN;
    rf[i].nneg  = 0;
    rf[i].npos  = 0;
    for ( int j = 0; j < n_dt; j++ ) { 
      if ( dt[j].tree_idx != i ) { continue; }
      rf[i].dt_lb = min(j, rf[i].dt_lb);
      rf[i].dt_ub = max(j, rf[i].dt_lb);
      rf[i].model_idx = dt[j].model_idx;
    }
    rf[i].dt_ub++; // convention that ub is exclusive
  }
  // TODO P4 Fairly dumb way to initialize mdl .Can be improved
  for ( int i = 0; i < n_mdl; i++ ) { 
    mdl[i].model_idx = i;
    mdl[i].rf_lb = INT_MAX;
    mdl[i].rf_ub = INT_MIN;
    mdl[i].prob = -1;
    for ( int j = 0; j < n_rf; j++ ) { 
      if ( rf[j].model_idx != i ) { continue; }
      mdl[i].rf_lb = min(j, mdl[i].rf_lb);
      mdl[i].rf_ub = max(j, mdl[i].rf_ub);
    }
    mdl[i].rf_ub++; // convention that ub is exclusive
  }
  for ( uint32_t i = 0; i < nC; i++ ) { 
    if ( file_exists(out_files[i]) ) { 
      status = remove(out_files[i]); 
      if ( status < 0 ) { WHEREAMI; }
      free_if_non_null(out_files[i]); 
    }
  }
  free_if_non_null(out_files);




BYE:
  mcr_rs_munmap(X, nX);
  for ( uint32_t i = 0; i < nC; i++ ) { free_if_non_null(fldtypes[i]); }
  if ( nil_files != NULL ) { 
    for ( uint32_t i = 0; i < nC; i++ ) { 
    free_if_non_null(nil_files[i]);
    }
  }
  free_if_non_null(nil_files);

  if ( out_files != NULL ) { 
    for ( uint32_t i = 0; i < nC; i++ ) { 
    free_if_non_null(out_files[i]);
    }
  }
  free_if_non_null(out_files);

  *ptr_dt = dt;
  *ptr_n_dt = n_dt;
  *ptr_rf = rf;
  *ptr_n_rf = n_rf;
  *ptr_mdl = mdl;
  *ptr_n_mdl = n_mdl;
  return status;
}
