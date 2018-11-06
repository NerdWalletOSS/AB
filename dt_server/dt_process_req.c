#include "dt_incs.h"
#include "dt_globals.h"
#include "dt_process_req.h"

#include "auxil.h"
#include "classify.h"
#include "get_from_lua.h"
#include "diagnostics.h"
#include "dump_log.h"
#include "init.h"
#include "load_models.h"
#include "l_make_feature_vector.h"
#include "l_post_proc_preds.h"
#include "setup.h"

// START FUNC DECL
int
dt_process_req(
    DT_REQ_TYPE req_type,
    const char *const api,
    const char *args,
    const char *body
    )
  // STOP FUNC DECL
{
  int status = 0;
  int num_features;
  const char *cptr = NULL;
  //-----------------------------------------
  memset(g_rslt, '\0', DT_MAX_LEN_RESULT+1);
  memset(g_err,  '\0', DT_ERR_MSG_LEN+1);
  memset(g_buf,  '\0', DT_ERR_MSG_LEN+1);
  //-----------------------------------------
  switch ( req_type ) {
    case Undefined :
      go_BYE(-1);
      break;
    case Classify : /* done by C */
      status = classify(body, g_rslt, DT_MAX_LEN_RESULT); cBYE(status);
      break;
      //--------------------------------------------------------
    case Diagnostics : /* done by C and Lua */
      status = diagnostics(args); cBYE(status);
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case DumpLog : /* done by C */
      status = dump_log(); cBYE(status);
      break;
      //--------------------------------------------------------
    case GetConfig : /* done by Lua */
      status = get_config(&cptr); cBYE(status);
      snprintf(g_rslt, DT_MAX_LEN_RESULT, "%s\n", cptr);
      break;
      //--------------------------------------------------------
    case GetNumFeatures : /* done by Lua */
      status = get_num_features(&num_features); cBYE(status);
      sprintf(g_rslt, " { \"NumFeatures\" : \"%d\" } \n", num_features);
      break;
      //--------------------------------------------------------
    case Halt : /* done by C */
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api);
      g_halt = true;
      break;
      //--------------------------------------------------------
    case HealthCheck :  /* done by C */
    case Ignore :  /* done by C */
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case LoadModels : 
      status = x_load_models(args); cBYE(status);
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case MakeFeatureVector : /* done by Lua */
      status = l_make_feature_vector(body);
      cBYE(status);
      break;
      //--------------------------------------------------------
    case MdlMeta : /* done by Lua */
      status = get_mdl_meta(&cptr); cBYE(status);
      snprintf(g_rslt, DT_MAX_LEN_RESULT, "%s\n", cptr);
      break;
      //--------------------------------------------------------
    case PostProcPreds : /* done by C */
      status = l_post_proc_preds(args, g_predictions, g_n_mdl,
          g_rslt, DT_MAX_LEN_RESULT);
      cBYE(status);
      break;
      //--------------------------------------------------------
    case ZeroCounters : /* done by C */
      zero_log();
      break;
      //--------------------------------------------------------
    default :
      go_BYE(-1);
      break;
  }
BYE:
  return status ;
}
