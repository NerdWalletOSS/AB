#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "l_hard_code_config.h"

int
l_hard_code_config()
{
  int status = 0;
  if ( g_disable_lua ) { return status; }
  if ( g_L == NULL ) { go_BYE(-1); }
  int num_lua_states = 2;
  for ( int i = 0; i <  num_lua_states; i++ ) {
    char state [4];
    lua_State *L = NULL;
    switch ( i ) {
      case 0 : L = g_L; strcpy(state, "AB"); break;
      case 1 : L = g_L_DT; strcpy(state, "DT"); break;
      default : go_BYE(-1); break;
    }
    lua_getglobal(L, "hard_code_config");
    if ( !lua_isfunction(L, -1)) {
      fprintf(stderr, "hard_code_config() does not exist in %s\n", state);
      lua_pop(L, 1);
      go_BYE(-1);
    }
    lua_pushlightuserdata(L, &g_cfg);
    status = lua_pcall(L, 1, 0, 0);
    if ( status != 0 ) {
      fprintf(stderr, "calling hard_code_config() for %s failed: %s\n", 
          state, lua_tostring(L, -1));
      sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(L, -1));
      lua_pop(L, 1);
      go_BYE(-1);
    }
  }
BYE:
  return status;
}
