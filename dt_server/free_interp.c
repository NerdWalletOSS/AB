#include "dt_incs.h"
#include "dt_types.h"
#include "free_interp.h"

//<hdr>
void
free_interp(
    DT_INTERPRETER_TYPE *interp
    )
  //</hdr>
{
  if ( interp != NULL ) {
    if ( ( interp->dt != NULL ) && ( interp->len_dt_file > 0 ) ) { 
      munmap(interp->dt, interp->len_dt_file); interp->n_dt = 0;
    }
    if ( ( interp->rf != NULL ) && ( interp->len_rf_file > 0 ) ) { 
      munmap(interp->rf, interp->len_rf_file); interp->n_rf = 0;
    }
    if ( ( interp->mdl != NULL ) && ( interp->len_mdl_file > 0 ) ) { 
      munmap(interp->mdl, interp->len_mdl_file); interp->n_mdl = 0;
    }
    free_if_non_null(interp->predictions); interp->n_mdl = 0;
    free_if_non_null(interp->dt_feature_vector); interp->n_dt_feature_vector = 0;
  }
}
