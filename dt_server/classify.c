#include "dt_incs.h"
#include "auxil.h"
#include "dt_globals.h"
#include "classify.h"
#include "l_make_feature_vector.h"
#include "l_post_proc_preds.h"
#include "eval_mdl.h"

int 
classify(
    const char *body,
    char *rslt,
    size_t sz_rslt
    )
{
  int status = 0;
  if ( body == NULL ) { go_BYE(-1); }
  status = l_make_feature_vector(body, false); cBYE(status);
  status = eval_mdl(g_dt_feature_vector, g_n_dt_feature_vector, 
      g_dt, g_n_dt, g_rf, g_n_rf, g_mdl, g_n_mdl, g_predictions);
  cBYE(status);
  status = l_post_proc_preds(NULL, g_predictions, g_n_mdl, rslt, sz_rslt);
  cBYE(status);
BYE:
  return status;
}
