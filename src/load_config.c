#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "load_config.h"
#include "update_config.h"

void restart_subsystems(char* has_changed) { //TODO declare prototype
  // For now a single restart is enough
  char changed = 0;
  for( int i=0; i< NUM_SERVICES; i++) {
    if (has_changed[i] == 1) {
      changed = 1;
      break;
    }
  }
  if( changed == 1) {
    update_config();
  }
}

int
l_hard_code_config()
{
  int status = 0;
  if ( g_disable_lua ) { return status; }
  lua_getglobal(g_L_DT, "hard_code");
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Function hard_code does not exist\n");
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  status = lua_pcall(g_L_DT, 0, 0, 0);
  if ( status != 0 ) {
    WHEREAMI;
    fprintf(stderr, "calling function hard_code for DT failed: %s\n", lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
BYE:
  return status;
}


int
l_load_config(
    const char *file_name
    )
{
  int status = 0;
  static char has_changed[NUM_SERVICES];
  memset(has_changed, 0, NUM_SERVICES);
  lua_getglobal(g_L, "load_config");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function load_config does not exist in lua's global space\n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  lua_pushlightuserdata(g_L, &g_cfg);
  lua_pushlightuserdata(g_L, has_changed);
  lua_pushstring(g_L, file_name); // not pushing string as it causes a copy
  status = lua_pcall(g_L, 3, 0, 0);
  if (status != 0) {
    WHEREAMI;
    fprintf(stderr, "calling function load_config failed: %s\n", lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
    go_BYE(-1);
  }

  // Now to load ML lua transforms

  lua_getglobal(g_L_DT, "load_config");
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Function load_config does not exist in DT lua's global space\n");
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  lua_pushstring(g_L_DT, file_name); // not pushing string as it causes a copy
  status = lua_pcall(g_L_DT, 1, 0, 0);
  if (status != 0) {
    WHEREAMI;
    fprintf(stderr, "calling function load_config for DT failed: %s\n", lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }

  restart_subsystems(has_changed);
BYE:
  return status;
}
