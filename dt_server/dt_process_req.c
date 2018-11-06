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
    case Diagnostics : 
      status = diagnostics(args); cBYE(status);
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case DumpLog : 
      status = dump_log(); cBYE(status);
      break;
      //--------------------------------------------------------
    case GetConfig : 
      status = get_config(&cptr); cBYE(status);
      snprintf(g_rslt, DT_MAX_LEN_RESULT, "%s\n", cptr);
      break;
      //--------------------------------------------------------
    case GetNumFeatures : 
      status = get_num_features(&g_n_dt_feature_vector); cBYE(status);
      sprintf(g_rslt, " { \"NumFeatures\" : \"%d\" } \n", g_n_dt_feature_vector);
      break;
      //--------------------------------------------------------
    case Halt : 
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api);
      g_halt = true;
      break;
      //--------------------------------------------------------
    case HealthCheck : 
    case Ignore :
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case LoadModels : 
      status = x_load_models(args); cBYE(status);
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case MakeFeatureVector : 
      /* Just for debugging */
      status = l_make_feature_vector(body);
      status =  get_num_features(&g_n_dt_feature_vector); cBYE(status);
      for ( int i = 0; i < g_n_dt_feature_vector; i++ ) { 
        fprintf(stderr,"%d: %f \n", i, g_dt_feature_vector[i]);
      }
      cBYE(status);
      break;
      //--------------------------------------------------------
    case MdlMeta : 
      status = get_mdl_meta(&cptr); cBYE(status);
      snprintf(g_rslt, DT_MAX_LEN_RESULT, "%s\n", cptr);
      break;
      //--------------------------------------------------------
    case PostProcPreds : /* just for testing */
      status = l_post_proc_preds(args, g_predictions, g_n_mdl,
          g_rslt, DT_MAX_LEN_RESULT);
      cBYE(status);
      break;
      //--------------------------------------------------------
    case ZeroCounters : 
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
