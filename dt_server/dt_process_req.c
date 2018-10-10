#include "dt_incs.h"
#include "auxil.h"
#include "dt_globals.h"
#include "dt_process_req.h"

#include "init.h"

#include "l_mdl_meta.h"
#include "l_get_num_features.h"
#include "l_make_feature_vector.h"
#include "l_post_proc_preds.h"

#include "reload.h"
#include "test_info.h"
#include "list_tests.h"
#include "get_config.h"

#include "num_tests.h"
#include "chk_logger_conn.h"
#include "kafka_check_conn.h"
#include "diagnostics.h"
#include "to_kafka.h"
#include "dump_log.h"
#include "add_fake_test.h"
#include "route_get_variant.h"
#include "ping_server.h"
#include "router.h"
#include "update_config.h"
#include "zero_globals.h"
#include "classify.h"
#include "classify_ua.h"
#include "ext_classify_ip.h"
#include "ext_classify_ua.h"
#include "setup.h"


#include "delete_test.h"
#include "stop_test.h"
#include "get_utm_kv.h"
#ifdef KAFKA
#include "kafka_close_conn.h"
#endif

extern char g_config_file[DT_MAX_LEN_FILE_NAME+1];

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
  char server[DT_MAX_LEN_SERVER_NAME+1];
  int num_features, itemp;
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
      status = get_config(); cBYE(status);
      break;
      //--------------------------------------------------------
    case GetNumFeatures : /* done by Lua */
      status = l_get_num_features(&num_features); cBYE(status);
      sprintf(g_rslt, " { \"NumFeatures\" : \"%d\", \"GNumfeatures\" : \"%d\" } \n", num_features, g_n_dt_feature_vector);
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
    case MakeFeatureVector : /* done by Lua */
      status = l_make_feature_vector(body, true);
      cBYE(status);
      break;
      //--------------------------------------------------------
    case MdlMeta : /* done by Lua */
      status = l_mdl_meta(); cBYE(status);
      break;
      //--------------------------------------------------------
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
