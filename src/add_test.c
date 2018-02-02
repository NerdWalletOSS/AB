#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "add_test.h"

// int cdata[1];
  int
l_add_test(
    const char *args
    )
{
  // cdata[0]=5;
  // printf("original value: %d\n", cdata[0]);
  int status = 0;
  lua_getglobal(g_L, "add");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function add does not exist in lua's global space\n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  lua_pushstring(g_L, args); // not pushing string as it causes a copy
  lua_pushlightuserdata(g_L, g_tests);
  status = lua_pcall(g_L, 2, 0, 0);
  if (status != 0) {
    fprintf(stderr, "calling function add failed: %s\n", lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
  }
  // printf("changed value: %d\n", cdata[0]);
BYE:
  return status;
}
