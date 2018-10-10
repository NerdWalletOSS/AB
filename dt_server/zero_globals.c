#include "dt_incs.h"
#include "dt_globals.h"
#include "zero_globals.h"
#include "auxil.h"
#include "dt_types.h"
extern DT_REC_TYPE *g_dt_map;
extern size_t g_len_dt_file;
extern uint32_t g_num_dt_map;

void
zero_cfg(
    void
    )
{
  memset(g_cfg.dt_dir, '\0', DT_MAX_LEN_FILE_NAME+1);
  memset(g_cfg.model_name, '\0', DT_MAX_LEN_FILE_NAME+1);

  g_cfg.port         = 0;
  g_cfg.verbose      = false;
  g_cfg.verbose      = false;
}

//<hdr>
void
free_globals(
    void
    )
  //</hdr>
{
  if ( ( g_dt != NULL ) && ( g_len_dt_file > 0 ) ) { 
    munmap(g_dt, g_len_dt_file); g_n_dt = 0;
  }
  if ( ( g_rf != NULL ) && ( g_len_rf_file > 0 ) ) { 
    munmap(g_rf, g_len_rf_file); g_n_rf = 0;
  }
  if ( ( g_mdl != NULL ) && ( g_len_mdl_file > 0 ) ) { 
    munmap(g_mdl, g_len_mdl_file); g_n_mdl = 0;
  }
  if ( !g_disable_lua ) { 
    if ( g_L    != NULL ) { lua_close(g_L);    g_L    = NULL; }
    if ( g_L_DT != NULL ) { lua_close(g_L_DT); g_L_DT = NULL; }
  }
  free_if_non_null(g_predictions); g_n_mdl = 0;
  free_if_non_null(g_dt_feature_vector); g_n_dt_feature_vector = 0;
}

int
zero_globals(
    void
    )
{
  int status = 0;

  g_halt = false;
  zero_cfg();
  memset(g_err, '\0', DT_ERR_MSG_LEN+1);
  memset(g_buf, '\0', DT_ERR_MSG_LEN+1);
  memset(g_rslt, '\0', DT_MAX_LEN_RESULT+1);

  g_dt_feature_vector = NULL;
  g_n_dt_feature_vector = 0;

  memset(g_valid_chars_in_url, '\0', 256);

  g_L    =  NULL;
  g_L_DT = NULL;
  g_dt  = NULL; g_n_dt = 0;
  g_rf  = NULL; g_n_rf = 0;
  g_mdl = NULL; g_n_mdl = 0;
  g_predictions = NULL;

  zero_log();
BYE:
  return status;
}

void
zero_log()
{
  g_log_start_time  = get_time_usec();

  g_log_no_user_agent  = 0;
  g_log_bad_user_agent = 0;
  g_log_bad_ab_args    = 0;

  g_log_ss_calls      = 0;
  g_log_ss_bad_code   = 0;
  g_log_ss_timeout    = 0;
  g_log_ss_non_ascii  = 0;
  g_log_ss_null_data  = 0;
  g_log_ss_bad_json   = 0;
  g_log_ss_no_session = 0;
  g_log_ss_bad_calls  = 0;

  g_log_missing_test  = 0;

  g_log_no_test_name  = 0; // GetVariant
  g_log_no_test_names = 0; // GetVariants
  g_log_no_uuid       = 0;
  g_log_no_test_type  = 0;
  g_log_no_tracer     = 0;

  g_log_dropped_posts     = 0;
  g_log_posts             = 0;
  g_log_bad_posts         = 0;
  g_log_failed_posts      = 0;

  g_log_bad_uuid      = 0;
  g_log_bad_test_type = 0;
  g_log_bad_test_name = 0;
  g_log_bad_tracer    = 0;

  g_log_get_alt_variant_calls = 0;
  g_log_get_variant_calls     = 0;
  g_log_get_variants_calls    = 0;
  g_log_router_calls          = 0;
  g_log_kafka_calls           = 0;
  g_log_kafka_total_time      = 0;
  g_log_bad_router_calls      = 0;

  g_log_num_probes     = 0;
  g_log_response_time  = 0;

  g_log_bad_test       = 0;
}
