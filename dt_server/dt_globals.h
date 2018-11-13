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

EXTERN char g_valid_chars_in_url[256]; // Set by C

//------------------------ For Lua
EXTERN lua_State *g_L_DT; // Set by C

#include "dt_log_globals.h"

#include "dt_types.h"

EXTERN DT_INTERPRETER_TYPE *g_interp;
