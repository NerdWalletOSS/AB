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
  static int entry_position[1];
  // cdata[0]=5;
  // printf("original value: %d\n", cdata[0]);
  int status = 0;
  lua_getglobal(g_L, "add");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function add does not exist in lua's global space\n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  entry_position[0] = -1;
  lua_pushstring(g_L, args); // not pushing string as it causes a copy
  lua_pushlightuserdata(g_L, g_tests);
  lua_pushlightuserdata(g_L, entry_position);
  status = lua_pcall(g_L, 3, 0, 0);
  if (status != 0) {
    fprintf(stderr, "calling function add failed: %s\n", lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
    // TODO memset the structure to 0 at entry_position
    if (entry_position[0] != -1) {
      memset(g_tests + entry_position[0], 0, sizeof(TEST_META_TYPE));
    }
    go_BYE(-1);
  }
  // printf("changed value: %d\n", cdata[0]);
BYE:
  return status;
}
