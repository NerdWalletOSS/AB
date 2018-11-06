#ifdef __DT_MAIN_PROGRAM
#define EXTERN extern
#else
#define EXTERN 
#endif
// Global State that persists across invocations
#include "dt_incs.h"
#include "dt_httpd_types.h"
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <pthread.h>

EXTERN bool g_halt; // For C TODO IS THIS NEEDED?
//-----------------------------------------------------------------
EXTERN char g_err[DT_ERR_MSG_LEN+1]; // For C: ab_process_req()
EXTERN char g_buf[DT_ERR_MSG_LEN+1]; // For C: ab_process_req()
EXTERN char g_rslt[DT_MAX_LEN_RESULT+1]; // For C: ab_process_req()
// above initialized as needed
//

EXTERN char g_body[DT_MAX_LEN_BODY+1];
EXTERN int g_sz_body;

EXTERN float *g_dt_feature_vector; 
EXTERN int g_n_dt_feature_vector  ;  // Set by Lua after reading configs

EXTERN char g_valid_chars_in_url[256]; // Set by C

//------------------------ For Lua
EXTERN lua_State *g_L_DT; // Set by C
EXTERN bool g_disable_lua; // used for testing, normally false


#include "dt_log_globals.h"

#include "dt_types.h"
EXTERN DT_REC_TYPE *g_dt; /* decision tree [g_n_dt]  */
EXTERN uint32_t g_n_dt;
EXTERN size_t g_len_dt_file; 
EXTERN RF_REC_TYPE *g_rf; /* random forest * [g_n_rf] */
EXTERN uint32_t g_n_rf;
EXTERN size_t g_len_rf_file; 
EXTERN MDL_REC_TYPE *g_mdl; /* models [g_n_mdl] */
EXTERN uint32_t g_n_mdl;
EXTERN size_t g_len_mdl_file; 
EXTERN float *g_predictions;  /* [g_n_mdl] */

