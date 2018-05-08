#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "l_update_config.h"

int 
l_update_config(
    void
    )
{
  int status = 0;

  if ( ( g_cfg.dt_dir == NULL ) || ( *g_cfg.dt_dir == '\0' ) ) { 
    return status; // Nothing to do 
  }
  // create separate Lua state for decision tree
  g_L_DT = luaL_newstate(); if ( g_L_DT == NULL ) { go_BYE(-1); }
  luaL_openlibs(g_L_DT);  
  // load necessary functions LUMPY TODO 
  status = luaL_loadfile(g_L_DT, "./dt.lua"); cBYE(status);
  status = lua_pcall(g_L_DT, 0, 0, 0); cBYE(status);
BYE:
  return status;
}
