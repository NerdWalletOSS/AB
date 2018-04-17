#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "url.h"
#include "ab_process_req.h"
#include "init.h"
#include "post_from_log_q.h"

#include "l_add_test.h"
#include "chk_logger_conn.h"
#include "chk_db_conn.h"
#include "diagnostics.h"
#include "dump_log.h"
// #include "hard_code_config.h"
#include "add_fake_test.h"
#include "route_get_variant.h"
#include "list_tests.h"
#include "ping_server.h"
#include "router.h"
#include "test_info.h"
#include "get_config.h"
#include "load_config.h"
#include "update_config.h"
#include "zero_globals.h"
#include "classify_ua.h"
#include "l_make_feature_vector.h"
#include "ext_get_host.h"
#include "l_post_proc_preds.h"

extern g_config_file[AB_MAX_LEN_FILE_NAME+1];

// START FUNC DECL
int 
ab_process_req(
    AB_REQ_TYPE req_type,
    const char *const api,
    const char *args,
    const char *body
    )
// STOP FUNC DECL
{
  int status = 0;
  char server[AB_MAX_LEN_SERVER_NAME+1];
  //-----------------------------------------
  memset(g_rslt, '\0', AB_MAX_LEN_RESULT+1);
  memset(g_err,  '\0', AB_ERR_MSG_LEN+1);
  memset(g_buf,  '\0', AB_ERR_MSG_LEN+1);
  //-----------------------------------------
  switch ( req_type ) {
    case Undefined : 
      go_BYE(-1);
      break;
      //--------------------------------------------------------
    case AddFakeTest : /* done by C */
      status = add_fake_test(args);  cBYE(status);
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case AddTest : /* done by Lua */
      status = l_add_test(body); cBYE(status);
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case CheckLoggerConnectivity :  /* done by C */
      status = chk_logger_connectivity(g_rslt, AB_MAX_LEN_RESULT); 
      cBYE(status);
      break;
      //--------------------------------------------------------
    case CheckDBConnectivity : /* done by Lua */
      status = l_chk_db_conn(); cBYE(status);
      break;
      //--------------------------------------------------------
    case ClassifyIP : /* done by C */
      status = ext_classify_ip(args, g_rslt,AB_MAX_LEN_RESULT); cBYE(status);
      break;
      //--------------------------------------------------------
    case ClassifyUA : /* done by C */
      status = ext_classify_ua(args, g_rslt,AB_MAX_LEN_RESULT); cBYE(status);
      break;
      //--------------------------------------------------------
    case Diagnostics : /* done by C and Lua */
      status = l_diagnostics(args); cBYE(status);
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api); 
      break;
      //--------------------------------------------------------
    case DumpLog : /* done by C */
      status = dump_log(args); cBYE(status);
      break;
      //--------------------------------------------------------
    case GetConfig : /* done by Lua */
      status = l_get_config(); cBYE(status);
      break;
      //--------------------------------------------------------
    case GetHost : /* done by Lua */
      status = ext_get_host(args, g_rslt, AB_MAX_LEN_RESULT); cBYE(status);
      break;
      //--------------------------------------------------------
    case GetVariant :  /* done by C */
    case GetVariants :  /* done by C */
      status = route_get_variant(req_type, args);  cBYE(status);
      break;
      //--------------------------------------------------------
    case Halt : /* done by C */
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api);
      g_halt = true;
      if ( g_cfg.sz_log_q > 0 ) { 
        // Tell consumer ead nothing more is coming 
        pthread_cond_signal(&g_condc);	/* wake up consumer */
        fprintf(stderr, "Waiting for consumer to finish \n");
        pthread_join(g_con, NULL);
        fprintf(stderr, "Consumer finished \n");
        pthread_mutex_destroy(&g_mutex);
        pthread_cond_destroy(&g_condc);
        pthread_cond_destroy(&g_condp);
      }
      break;
      //--------------------------------------------------------
    case HealthCheck :  /* done by C */
    case Ignore :  /* done by C */
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case ListTests : /* done by Lua */
      status = l_list_tests(args); cBYE(status);
      break;
      //--------------------------------------------------------
    case LoadConfig : /* done by Lua */
      status = l_load_config(body); cBYE(status);
      break;
      //--------------------------------------------------------
    case MakeFeatureVector : /* done by Lua */
      status = l_make_feature_vector(body, true); 
      cBYE(status);
      break;
      //--------------------------------------------------------
    case PingServer : /* done by C */
      memset(server, '\0', AB_MAX_LEN_SERVER_NAME);
      status = extract_name_value(args, "Service=", '&', 
          server, AB_MAX_LEN_SERVER_NAME);
      cBYE(status);
      if ( *server == '\0' ) { go_BYE(-1); }
      if ( strcmp(server, "logger") == 0 ) { 
        status = ping_server("logger", g_cfg.logger.server, 
            g_cfg.logger.port, g_cfg.logger.health_url, g_rslt);
      }
      else if ( strcmp(server, "logger") == 0 ) { 
        status = ping_server("logger", g_cfg.ss.server, 
            g_cfg.ss.port, g_cfg.ss.health_url, g_rslt);
      }
      else if ( strcmp(server, "logger") == 0 ) { 
        status = ping_server("logger", g_cfg.webapp.server, 
            g_cfg.webapp.port, g_cfg.webapp.health_url, g_rslt);
      }
      else {
        go_BYE(-1);
      }
      break;
      //--------------------------------------------------------
    case PostProcPreds : /* done by C */
      status = l_post_proc_preds(g_predictions, g_n_mdl, 
          g_rslt, AB_MAX_LEN_RESULT);
      cBYE(status);
      break;
      //--------------------------------------------------------
    case Reload : /* done by Lua */
    case Restart : /* done by Lua */
      // t1 = get_time_usec(); 
      g_halt = true;
      if ( g_cfg.sz_log_q > 0 ) { 
        // Tell consumer thread nothing more is coming 
        pthread_cond_signal(&g_condc);	/* wake up consumer */
        fprintf(stderr, "Waiting for consumer to finish \n");
        pthread_join(g_con, NULL);
        fprintf(stderr, "Consumer finished \n");
        pthread_mutex_destroy(&g_mutex);
        pthread_cond_destroy(&g_condc);
        pthread_cond_destroy(&g_condp);
      }
      g_halt = false;
      // common to restart and reload
      free_globals();
      status = zero_globals();  cBYE(status);
      status = init_lua(); cBYE(status);
      if ( g_config_file[0] == '\0' )  {
        hard_code_config(); // only for testing 
        status = l_hard_code_config(); cBYE(status); // only for testing 
      }
      else {
        status = l_load_config(g_config_file); cBYE(status);
      }
      status = update_config(); cBYE(status);
      if ( g_cfg.sz_log_q > 0 ) { 
        pthread_mutex_init(&g_mutex, NULL);	
        pthread_cond_init(&g_condc, NULL);
        pthread_cond_init(&g_condp, NULL);
        status = pthread_create(&g_con, NULL, &post_from_log_q, NULL);
        cBYE(status);
      }
      switch ( req_type ) { 
        case Reload: /* call Lua status = reload(false); */ break;
        case Restart : /* nothing to do  */ break;
        default : go_BYE(-1); break;
      }
      cBYE(status);
      sprintf(g_rslt, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case Router : /* done by C */
      status = router(args); cBYE(status);
      break;
      //--------------------------------------------------------
    case TestInfo : /* done by Lua */
      status = l_test_info(args); cBYE(status);
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
