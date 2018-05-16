#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "aux_zero.h"
#include "l_reload_tests.h"

// int cdata[1];
  int
l_reload_tests()
{
   for(int i=0; i < AB_MAX_NUM_TESTS; i++){
    g_reload_tests[i] = NULL;
  }
  static int num_entries[1];
  int status = 0;
  num_entries[0] = -1;
  lua_getglobal(g_L, "reload");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function add does not exist in lua's global space\n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  num_entries[0] = -1;
  lua_pushlightuserdata(g_L, num_entries);
  lua_pushlightuserdata(g_L, g_reload_tests);
  status = lua_pcall(g_L, 2, 0, 0);
  if (status != 0) {
    WHEREAMI; 
    fprintf(stderr, "calling function add failed: %s\n", lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
    // TODO memset the structure to 0 at num_entries, INDRAJEET+RAMESH
    go_BYE(-1);
  }
  if (num_entries[0] <= 0){
    WHEREAMI;
    go_BYE(-1);
  }
  // TODO free all tests
  for(int i=0; i<num_entries[0]; i++){
    status = l_add_test(g_reload_tests[i]);
  }
  // printf("changed value: %d\n", cdata[0]);
BYE:
  for(int i=0; i < AB_MAX_NUM_TESTS; i++){
    g_reload_tests[i] = NULL;
  }
  return status;
}
