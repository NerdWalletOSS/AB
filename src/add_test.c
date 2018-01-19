#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "add_test.h"

int 
l_add_test(
    const char *args
    )
{
  int * cdata = malloc(sizeof(int));
  cdata[0]=5;
  printf("original value: %d\n", cdata[0]);
  int status = 0;
  status = luaL_dostring(g_L, "return require 'add'");
  if (status != 0) {
    fprintf(stderr, "Running require failed: %s\n", lua_tostring(g_L,-1));
    lua_pop(g_L, 1);
  } else if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Running require did not return a function \n");
    lua_pop(g_L, 1);
  }
  cBYE(status); 
  lua_pushstring(g_L, args);
  lua_pushlightuserdata(g_L, cdata);
  status = lua_pcall(g_L, 2, 0, 0);
  if (status != 0) {
    fprintf(stderr, "calling function add failed: %s\n", lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
  }
  printf("changed value: %d\n", cdata[0]);
BYE:
  free(cdata);
  return status;
}
