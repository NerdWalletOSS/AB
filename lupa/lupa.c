
#include "dt_incs.h"
#include "lupa.h"
#include "load_models.h"
#include "init.h"
#include "eval_mdl.h"

DT_INTERPRETER_TYPE *g_interp;

int
init(
    const char *const dt_dir,
    const char *const model_name
    )
{
  int status = 0;

  g_interp = malloc(1 * sizeof(DT_INTERPRETER_TYPE));
  return_if_malloc_failed(g_interp);
  memset(g_interp, '\0', sizeof(DT_INTERPRETER_TYPE));

  status = load_models(dt_dir, model_name, g_interp); cBYE(status);
BYE:
  return status;
}

void
release(
    void
    )
{
  free_interp(g_interp);
  free_if_non_null(g_interp);
}

int
eval(
    void
    )
{
  int status = 0;
  status = eval_mdl(
      g_interp->dt_feature_vector, 
      g_interp->n_dt_feature_vector, 
      g_interp->dt, g_interp->n_dt, 
      g_interp->rf, g_interp->n_rf, 
      g_interp->mdl, g_interp->n_mdl, 
      g_interp->predictions);
BYE:
  return status;
}
