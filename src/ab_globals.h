#ifdef __AB_MAIN_PROGRAM
#define EXTERN extern
#else
#define EXTERN 
#endif
// Global State that persists across invocations
#include "unistd.h"
#include "spooky_hash.h"
#include "statsd-client.h"
#include <curl/curl.h>
#include <semaphore.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <pthread.h>

/* START: Following are initialized by zero_conf() */
EXTERN uint16_t  g_port; // my port
EXTERN bool g_verbose;
EXTERN bool g_log_csv;
//
//--------  For logging 
EXTERN uint16_t  g_log_port;
EXTERN char g_log_server[AB_MAX_LEN_SERVER_NAME+1];
EXTERN char g_log_url[AB_MAX_LEN_URL+1];
EXTERN char g_log_health_url[AB_MAX_LEN_URL+1];
//-----------------------------------------------------
//--------  For session sevice
EXTERN uint16_t  g_ss_port;
EXTERN char g_ss_server[AB_MAX_LEN_SERVER_NAME+1];
EXTERN char g_ss_url[AB_MAX_LEN_URL+1];
EXTERN char g_ss_health_url[AB_MAX_LEN_URL+1];
//
EXTERN char g_statsd_host[AB_MAX_LEN_SERVER_NAME+1];
EXTERN int g_statsd_port;
EXTERN char g_statsd_inc[AB_MAX_LEN_URL+1];
EXTERN char g_statsd_timing[AB_MAX_LEN_URL+1];

EXTERN uint32_t g_sz_log_q;  // maximum number of spots in q
EXTERN int g_num_post_retries; 
EXTERN uint32_t   g_sz_uuid_ht; /* size of uuid hash table */
EXTERN bool g_reload_on_startup;
EXTERN char g_default_url[AB_MAX_LEN_REDIRECT_URL+1];
EXTERN uint64_t g_xy_guid;
EXTERN bool g_test_uuid_len;
EXTERN int g_uuid_len;
/* Above     are initialized by zero_conf() */
//-----------------------------------------------------
/* Following are initialized by zero_globals() */
EXTERN char *g_ss_response;
EXTERN int  g_sz_ss_response;
//-----------------------------------------------------
EXTERN statsd_link *g_statsd_link;


EXTERN uint32_t g_n_log_q;   // number of spots used in q
EXTERN PAYLOAD_TYPE *g_log_q; // the q itself
EXTERN uint32_t g_q_rd_idx; // spot where producer is to write
EXTERN uint32_t g_q_wr_idx; // spot where consumer is to read

EXTERN char *g_uuid;

EXTERN char g_my_name[AB_MAX_LEN_HOSTNAME+1];
//---- STOP: Above initialized in zero_globals()

//--- START: Initialized in test_init()
EXTERN bool g_halt;
EXTERN unsigned long long g_t_start; // for performance metrics
//--- START: Initialized in test_init()
//-----------------------------------------------------------------
EXTERN TEST_META_TYPE g_tests[AB_MAX_NUM_TESTS];  // set by Lua 
//--------------------------------------------------------
EXTERN uint64_t g_seed1;
EXTERN uint64_t g_seed2;
EXTERN spooky_state g_spooky_state;
//--------------------------------------------------------
//--------------------------------------------------------
EXTERN CURL  *g_ch ;
EXTERN struct curl_slist *g_curl_hdrs;

EXTERN CURL  *g_ss_ch ; // used to talk to session service
EXTERN struct curl_slist *g_ss_curl_hdrs; // used for session service


// Following initialized as needed
EXTERN char g_redirect_url[AB_MAX_LEN_REDIRECT_URL+1]; // router()
EXTERN char g_err[AB_ERR_MSG_LEN+1]; // ab_process_req()
EXTERN char g_buf[AB_ERR_MSG_LEN+1]; // ab_process_req()
EXTERN char g_rslt[AB_MAX_LEN_RESULT+1]; // ab_process_req()
EXTERN char g_mrslt[AB_MAX_LEN_RESULT+1]; // for GetVariants
EXTERN char g_curl_payload[AB_MAX_LEN_PAYLOAD+1]; // post_from_log_q()
// above initialized as needed
//
// Stuff needed for multi-threading 
EXTERN pthread_t g_con;
EXTERN pthread_cond_t g_condc, g_condp;
EXTERN pthread_mutex_t g_mutex;

EXTERN uint32_t g_device_idx; // for device specific routing URL Router
EXTERN DEV_REC_TYPE *g_devices; // Set by Lua 
EXTERN uint32_t g_n_devices; // Set by Lua 
EXTERN uint32_t g_other_idx; // Set by Lua used for user-agents that do not match

EXTERN char g_ua_to_dev_map_file[AB_MAX_LEN_FILE_NAME+1];
EXTERN uint64_t *g_ua_to_dev_map;
EXTERN uint32_t g_n_ua_to_dev_map;

EXTERN char g_valid_chars_in_url[256];
//------------------------ For Lua
EXTERN lua_State *g_L;
//--- For Internal log 
EXTERN char *g_log;
EXTERN uint32_t g_num_log_entries;
EXTERN uint64_t g_log_idx;
EXTERN uint64_t g_log_time;

EXTERN uint64_t g_log_start_time;

EXTERN uint64_t g_log_router_calls;
EXTERN uint64_t g_log_no_user_agent;
EXTERN uint64_t g_log_bad_user_agent;

EXTERN uint64_t g_log_ss_calls;
EXTERN uint64_t g_log_ss_non_ascii;
EXTERN uint64_t g_log_ss_bad_code;
EXTERN uint64_t g_log_ss_no_session;
EXTERN uint64_t g_log_ss_bad_json;

EXTERN uint64_t g_log_missing_test_ab;
EXTERN uint64_t g_log_missing_test_xy;

EXTERN uint64_t g_log_no_test_name; // GetVariant
EXTERN uint64_t g_log_no_test_names; // GetVariants

EXTERN uint64_t g_log_posts; // num posts attempted (retries not counted)
EXTERN uint64_t g_log_bad_posts; // to Logger => total failure
EXTERN uint64_t g_log_failed_posts; // to Logger => might success on retry

/* allow some retries. If a post makes 2 attempts and fails on both,
then num_bad_posts is incremented by 1 and num_failed_posts by 2. If
it succeeds on second attempt, then num_failed_posts increments by 1
and num_bad_posts stays as is
*/

EXTERN uint64_t g_log_bad_uuid; 
EXTERN uint64_t g_log_bad_test_type;
EXTERN uint64_t g_log_bad_test_name;

EXTERN uint64_t g_num_get_variant_calls; // for PerfMetrics 
EXTERN uint64_t g_num_bad_xy_calls; // for PerfMetrics 
EXTERN uint64_t g_num_posts; // for PerfMetrics 
// difference between bad posts and failed posts is as follows. Note that we
