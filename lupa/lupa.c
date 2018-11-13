
#include "dt_incs.h"
#include "lupa.h"
#include "load_models.h"

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

