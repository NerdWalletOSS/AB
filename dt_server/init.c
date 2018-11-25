#include <lua.h>
#include "dt_incs.h"
#include "dt_globals.h"
#include "init.h"
#include "auxil.h"
#include "dt_types.h"
extern lua_State *g_L_DT; 
extern char g_err[DT_ERR_MSG_LEN+1]; 

void
free_globals(
    void
    )
{
  free_interp(g_interp);
  free_if_non_null(g_interp);
  if ( g_L_DT != NULL ) { lua_close(g_L_DT); g_L_DT = NULL; }
}

//<hdr>
void
free_interp(
    DT_INTERPRETER_TYPE *interp
    )
  //</hdr>
{
  if ( interp != NULL ) {
    if ( ( interp->dt != NULL ) && ( interp->len_dt_file > 0 ) ) { 
      munmap(interp->dt, interp->len_dt_file); interp->n_dt = 0;
    }
    if ( ( interp->rf != NULL ) && ( interp->len_rf_file > 0 ) ) { 
      munmap(interp->rf, interp->len_rf_file); interp->n_rf = 0;
    }
    if ( ( interp->mdl != NULL ) && ( interp->len_mdl_file > 0 ) ) { 
      munmap(interp->mdl, interp->len_mdl_file); interp->n_mdl = 0;
    }
    free_if_non_null(interp->predictions); interp->n_mdl = 0;
    free_if_non_null(interp->dt_feature_vector); interp->n_dt_feature_vector = 0;
  }
}

void
zero_globals(
    void
    )
{
  g_halt = false;
  memset(g_err, '\0', DT_ERR_MSG_LEN+1);
  memset(g_buf, '\0', DT_ERR_MSG_LEN+1);
  memset(g_rslt, '\0', DT_MAX_LEN_RESULT+1);

  //------------
  const char *str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ=/_:";
  memset(g_valid_chars_in_url, '\0', 256);
  for ( char *cptr = (char *)str; *cptr != '\0'; cptr++ ) {
    g_valid_chars_in_url[(uint8_t)(*cptr)] = true;
  }

  zero_log();
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
    const char * const config_file
    )
{
  int status = 0;
  g_L_DT = luaL_newstate(); if ( g_L_DT == NULL ) { go_BYE(-1); }
  luaL_openlibs(g_L_DT);  
  status = luaL_dostring(g_L_DT, "require 'DT/init'"); 
  if ( status != 0 ) { 
    fprintf(stderr, "Lua load : %s\n", lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  cBYE(status);
  //--- Now process the config file 
  const char *const lua_fn = "proc_config_file";
  lua_getglobal(g_L_DT, lua_fn);
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua Function %s undefined\n", lua_fn);
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  lua_pushstring(g_L_DT, config_file);
  status = lua_pcall(g_L_DT, 1, 1, 0);
  if ( status != 0 ) {
    fprintf(stderr, "calling function %s failed: %s\n", 
        lua_fn, lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  if (!lua_isboolean(g_L_DT, -1)) {
    fprintf(stderr, "%s: return 1 must be a boolean\n", __func__); go_BYE(-1); 
  }
  bool l_status = lua_toboolean(g_L_DT, -1);
  if ( l_status != true ) { go_BYE(-1); }
  // --------------------
BYE:
  return status;
}
