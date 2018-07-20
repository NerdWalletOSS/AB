#include "ab_incs.h"
#include "auxil.h"

#include "aux_zero.h"
#include "reload.h"
#include <lua.h>

#define AB_MAX_LEN_TEST_AS_STR 16384-1

extern char g_rslt[AB_MAX_LEN_RESULT+1];
extern char g_err[AB_MAX_LEN_RESULT+1];
extern lua_State *g_L;
extern CFG_TYPE g_cfg;

int
reload(
    void
    )
{
  int status = 0;
  if ( g_L == NULL ) { go_BYE(-1); }
  char test_as_str[AB_MAX_LEN_TEST_AS_STR+1];
  const char *db   = g_cfg.mysql.database;
  const char *host = g_cfg.mysql.server;
  const char *user = g_cfg.mysql.user;
  const char *pass = g_cfg.mysql.password;
  int         port = g_cfg.mysql.port;
  int         tid[1]; tid[0] = 0; // tracks progress through tests id

  lua_getglobal(g_L, "reload");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function add does not exist in lua's global space\n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++){
    free_test(i);
  }
  for ( ; ; ) { 
    lua_pushstring(g_L, db);
    lua_pushstring(g_L, host);
    lua_pushstring(g_L, user);
    lua_pushstring(g_L, pass);
    lua_pushnumber(g_L, port);
    lua_pushlightuserdata(g_L, tid);
    lua_pushlightuserdata(g_L, test_as_str);
    status = lua_pcall(g_L, 7, 0, 0);
    if (status != 0) {
      fprintf(stderr, "Lua function reload failed: %s\n", 
          lua_tostring(g_L, -1));
      sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
      lua_pop(g_L, 1);
      go_BYE(-1);
    }
    status = add_test(test_as_str); cBYE(status);
  }
BYE:
  return status;
}
