#include "ab_incs.h"
#include "auxil.h"

#include "aux_zero.h"
#include "reload.h"
#include <lua.h>


extern char g_rslt[AB_MAX_LEN_RESULT+1];
extern char g_err[AB_MAX_LEN_RESULT+1];
extern lua_State *g_L;
extern CFG_TYPE g_cfg;
extern uint64_t g_log_bad_test;

int
reload(
    void
    )
{
  int status = 0;
  if ( g_L == NULL ) { go_BYE(-1); }
  const char *host = g_cfg.mysql.host;
  const char *user = g_cfg.mysql.user;
  const char *pass = g_cfg.mysql.pass;
  const char *db   = g_cfg.mysql.db;
  uint64_t    port = g_cfg.mysql.port;

  for ( int i = 0; i < AB_MAX_NUM_TESTS; i++){
    free_test(i);
  }
  uint64_t tid;
  for ( ; ; ) {
    lua_getglobal(g_L, "get_test");
    if ( !lua_isfunction(g_L, -1)) {
      fprintf(stderr, "Function add does not exist in lua's global space\n");
      lua_pop(g_L, 1);
      go_BYE(-1);
    }
    lua_pushstring(g_L, db);
    lua_pushstring(g_L, host);
    lua_pushstring(g_L, user);
    lua_pushstring(g_L, pass);
    lua_pushnumber(g_L, port);
    lua_pushnumber(g_L, tid);
    status = lua_pcall(g_L, 6, 2, 0);
    if (status != 0) {
      fprintf(stderr, "Lua function get_test failed: %s\n", 
          lua_tostring(g_L, -1));
      sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
      lua_pop(g_L, 1);
      go_BYE(-1);
    }
    // get test id
    if (!lua_isnumber(g_L, -2)) {
      fprintf(stderr, "reload: 1st return must be a number"); go_BYE(-1); 
    }
    tid = lua_tonumber(g_L, -2);
    if ( tid == 0 ) { break; } // no more tests
    // get test string
    if (!lua_isstring(g_L, -1)) {
      fprintf(stderr, "reload: 2nd return must be a string"); go_BYE(-1); 
    }
    const char *test_as_str = lua_tostring(g_L, -1);
    status = add_test(test_as_str);
    if ( status < 0 ) { // don't quit if one test is bad
      fprintf(stderr, "Bad test %s \n", test_as_str);
      g_log_bad_test++; // TODO P2 statsd
    }
    //---------------------
    lua_settop(g_L, 0); // lua_pop(g_L, 2);
  }
BYE:
  return status;
}
