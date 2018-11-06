#include <lua.h>
#include "dt_incs.h"
#include "dt_globals.h"
#include "init.h"
#include "auxil.h"
#include "dt_types.h"
extern lua_State *g_L_DT; // Set by C

extern DT_REC_TYPE *g_dt_map;
extern size_t g_len_dt_file;
extern uint32_t g_num_dt_map;

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
  memset(g_err, '\0', DT_ERR_MSG_LEN+1);
  memset(g_buf, '\0', DT_ERR_MSG_LEN+1);
  memset(g_rslt, '\0', DT_MAX_LEN_RESULT+1);

  g_dt_feature_vector = NULL;
  g_n_dt_feature_vector = 0;

  g_L_DT = NULL;
  g_dt  = NULL; g_n_dt = 0;
  g_rf  = NULL; g_n_rf = 0;
  g_mdl = NULL; g_n_mdl = 0;
  g_predictions = NULL;

  zero_log();
  return status;
}

void
zero_log(
    void
    )
{
  g_log_start_time  = get_time_usec();

  g_log_response_time  = 0;
  g_log_num_classify  = 0;
}

int
init_lua(
    void
    )
{
  int status = 0;
  g_L_DT = luaL_newstate(); if ( g_L_DT == NULL ) { go_BYE(-1); }
  luaL_openlibs(g_L_DT);  
  status = luaL_dostring(g_L_DT, "require 'DT/dt'"); 
  if ( status != 0 ) { 
    fprintf(stderr, "Lua load : %s\n", lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  cBYE(status);

BYE:
  return status;
}
