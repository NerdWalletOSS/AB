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
    lua_getglobal(L, "update_config");
    if ( !lua_isfunction(L, -1)) {
      fprintf(stderr, "update_config() does not exist in %s\n", state);
      lua_pop(L, 1);
      go_BYE(-1);
    }
    status = lua_pcall(L, 0, 0, 0);
    if ( status != 0 ) {
      fprintf(stderr, "calling update_config() for %s failed: %s\n", 
          state, lua_tostring(L, -1));
      sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(L, -1));
      lua_pop(L, 1);
      go_BYE(-1);
    }
  }
BYE:
  return status;
}
