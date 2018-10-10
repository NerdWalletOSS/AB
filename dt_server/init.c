#include "dt_incs.h"
#include "dt_globals.h"

#include "auxil.h"
#include "zero_globals.h"
#include "init.h"

int
init_lua(
    void
    )
{
  int status = 0;
  if ( g_disable_lua ) { return status; }
  g_L = luaL_newstate(); if ( g_L == NULL ) { go_BYE(-1); }
  luaL_openlibs(g_L);  
  status = luaL_dostring(g_L, "require 'RTS/ab'"); cBYE(status);

  if ( !g_disable_dt ) { 
    g_L_DT = luaL_newstate(); if ( g_L_DT == NULL ) { go_BYE(-1); }
    luaL_openlibs(g_L_DT);  
    status = luaL_dostring(g_L_DT, "require 'DT/dt'"); cBYE(status);
  }

BYE:
  return status;
}

void
init_globals(
    void
    )
{

  // TODO Check with Braad that this is good
  const char *url_str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ&=/:_-%,;[].?+() ";
  for ( char *cptr = (char *)url_str; *cptr != '\0'; cptr++ ) {
    g_valid_chars_in_url[(uint8_t)(*cptr)] = true;
  }
}
