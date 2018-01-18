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

EXTERN uint16_t  g_port; // Lua config
EXTERN bool g_verbose; // Lua config
//
//--------  For logging 
EXTERN uint16_t  g_log_port; // Lua config 
EXTERN char g_log_server[AB_MAX_LEN_SERVER_NAME+1]; // Lua config 
EXTERN char g_log_url[AB_MAX_LEN_URL+1]; // Lua config 
EXTERN char g_log_health_url[AB_MAX_LEN_URL+1]; // Lua config 
//-----------------------------------------------------
//--------  For session sevice
EXTERN uint16_t  g_ss_port; // Lua config 
EXTERN char g_ss_server[AB_MAX_LEN_SERVER_NAME+1]; // Lua config 
EXTERN char g_ss_url[AB_MAX_LEN_URL+1]; // Lua config 
EXTERN char g_ss_health_url[AB_MAX_LEN_URL+1]; // Lua config
//
EXTERN char g_statsd_host[AB_MAX_LEN_SERVER_NAME+1]; // Lua config
EXTERN int g_statsd_port; // Lua config
EXTERN char g_statsd_inc[AB_MAX_LEN_URL+1]; // Lua config
EXTERN char g_statsd_timing[AB_MAX_LEN_URL+1]; // Lua config

EXTERN uint32_t g_sz_log_q;  // size of queue of pending post requests Lua config
EXTERN int g_num_post_retries;  // Lua config 
EXTERN bool g_reload_on_startup; // Lua config  TODO
EXTERN char g_default_url[AB_MAX_LEN_REDIRECT_URL+1]; // Lua config 

EXTERN char *g_ss_response; // For C. Get response from Session Server
EXTERN int  g_sz_ss_response; // For C. Size of response

EXTERN statsd_link *g_statsd_link; // For C to talk to statsd server

EXTERN uint32_t g_n_log_q;   // For C
EXTERN PAYLOAD_TYPE *g_log_q; // For C
EXTERN uint32_t g_q_rd_idx; // For C: spot where producer is to write
EXTERN uint32_t g_q_wr_idx; // For C: spot where consumer is to read

EXTERN int g_uuid_len; // Lua config 
EXTERN char *g_uuid; // For C
EXTERN uint64_t g_xy_guid; // For C. Set to positive integer for debugging. 
// Normally this should be 0

EXTERN char g_my_name[AB_MAX_LEN_HOSTNAME+1]; // For C

EXTERN bool g_halt; // For C
//-----------------------------------------------------------------
EXTERN TEST_META_TYPE g_tests[AB_MAX_NUM_TESTS];  // Set by Lua 
//--------------------------------------------------------
EXTERN uint64_t g_seed1; // For C
EXTERN uint64_t g_seed2; // For C
EXTERN spooky_state g_spooky_state; // For C

EXTERN CURL  *g_ch ; // For C: to talk to Logger
EXTERN struct curl_slist *g_curl_hdrs; // For C: to talk to Logger

EXTERN CURL  *g_ss_ch ; // For C: to talk to session service
EXTERN struct curl_slist *g_ss_curl_hdrs; // For C: to talk to session service

// Following initialized as needed
EXTERN char g_redirect_url[AB_MAX_LEN_REDIRECT_URL+1]; // For C: router()
EXTERN char g_err[AB_ERR_MSG_LEN+1]; // For C: ab_process_req()
EXTERN char g_buf[AB_ERR_MSG_LEN+1]; // For C: ab_process_req()
EXTERN char g_rslt[AB_MAX_LEN_RESULT+1]; // For C: ab_process_req()
EXTERN char g_mrslt[AB_MAX_LEN_RESULT+1]; // For C: for GetVariants
EXTERN char g_curl_payload[AB_MAX_LEN_PAYLOAD+1]; // For C: post_from_log_q()
// above initialized as needed
//
// Stuff needed for multi-threading 
EXTERN pthread_t g_con; // For C
EXTERN pthread_cond_t g_condc, g_condp; // For C
EXTERN pthread_mutex_t g_mutex; // For C

EXTERN uint32_t g_device_idx; // For C: for device specific routing URL Router
EXTERN DEV_REC_TYPE *g_devices; // Set by Lua 
EXTERN uint32_t g_n_devices; // Set by Lua 
EXTERN uint32_t g_other_idx; // Set by Lua used for user-agents that do not match

EXTERN char g_ua_to_dev_map_file[AB_MAX_LEN_FILE_NAME+1]; // Set by Lua
EXTERN uint64_t *g_ua_to_dev_map; // Set by C
EXTERN size_t g_n_ua_to_dev_map; // Set by C
EXTERN uint32_t g_num_ua_to_dev_map; // Set by C

EXTERN char g_valid_chars_in_url[256]; // Set by C
EXTERN char g_valid_chars_in_ua[256]; // Set by C
//------------------------ For Lua
EXTERN lua_State *g_L; // Set by C

#include "ab_log_globals.h"
