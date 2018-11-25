#include "dt_incs.h"
#include "auxil.h"

#include "classify.h"
#include "l_make_feature_vector.h"
#include "l_post_proc_preds.h"
#include "eval_mdl.h"

int 
classify(
    DT_INTERPRETER_TYPE *g_interp,
    const char *body,
    char *rslt, /* [sz_rslt] */
    size_t sz_rslt
    )
{
  int status = 0;
  if ( body == NULL ) { go_BYE(-1); }
  status = l_make_feature_vector(body); cBYE(status);
  status = eval_mdl(
      g_interp->dt_feature_vector, 
      g_interp->n_dt_feature_vector, 
      g_interp->dt, g_interp->n_dt, 
      g_interp->rf, g_interp->n_rf, 
      g_interp->mdl, g_interp->n_mdl, 
      g_interp->forest_type, g_interp->predictions);
  cBYE(status);
  status = l_post_proc_preds(NULL, 
      g_interp->predictions, 
      g_interp->n_mdl);
  cBYE(status);
BYE:
  return status;
}
