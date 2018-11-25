#include "dt_incs.h"
#include "auxil.h"

#include "classify.h"
#include "l_make_feature_vector.h"
#include "l_post_proc_preds.h"
#include "eval_mdl.h"

int
lua_eval_mdl(
    DT_INTERPRETER_TYPE *interp
    )
{
  int status = 0;
  status = eval_mdl(
      interp->dt_feature_vector, 
      interp->n_dt_feature_vector, 
      interp->dt, interp->n_dt, 
      interp->rf, interp->n_rf, 
      interp->mdl, interp->n_mdl, 
      interp->forest_type, interp->predictions);
BYE:
  return status;
}
//---------------------------------------------
int 
classify(
    DT_INTERPRETER_TYPE *interp,
    const char *body,
    char *rslt, /* [sz_rslt] */
    size_t sz_rslt
    )
{
  int status = 0;
  if ( body == NULL ) { go_BYE(-1); }
  status = l_make_feature_vector(body); cBYE(status);
  status = lua_eval_mdl(interp); cBYE(status);
  status = l_post_proc_preds(NULL, 
      interp->predictions, 
      interp->n_mdl);
  cBYE(status);
BYE:
  return status;
}
