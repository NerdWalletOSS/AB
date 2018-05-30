#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "aux_zero.h"
#include "l_add_test.h"

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
  //-------------------------------------
  if ( g_L == NULL ) { go_BYE(-1); }
  int32_t rslt[4]; 
  lua_getglobal(g_L, "preproc");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function preproc does not exist \n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  entry_position[0] = -1;
  lua_pushstring(g_L, args); // not pushing string as it causes a copy
  lua_pushlightuserdata(g_L, g_tests);
  lua_pushlightuserdata(g_L, rslt);
  status = lua_pcall(g_L, 3, 0, 0);
  if (status != 0) {
    fprintf(stderr, "function preproc failed: %s\n", lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  // Now based on rslt, do things
  int what_to_do      = rslt[0];
  int test_idx        = rslt[1]; 
  int num_variants    = rslt[2];
  int is_dev_specific = rslt[3];
  switch ( what_to_do ) { 
    case 1: // nil to started
      status = malloc_test(test_idx, num_variants, is_dev_specific,
          TEST_STATE_STARTED);
      break;
    case 2: // nil to terminated
      status = malloc_test(test_idx, num_variants, is_dev_specific,
          TEST_STATE_TERMINATED);
      break;
    case 3: // nil to archived => Nothing to do
      break;
    case 4: // stared to started => Nothing to do
      break;
    case 5: // stared to terminated
      status = free_variant_per_bin(test_idx);
      break;
    case 6: // started to archived
      status = free_test(test_idx); 
      break;
    case 7: // terminated to started
      go_BYE(-1); 
      break;
    case 8: // terminated to terminated
      go_BYE(-1); 
      break;
    case 9: // terminated to archived
      status = free_test(test_idx); 
      break;
    default:
      go_BYE(-1);
      break;
  }
  cBYE(status);
  //-------------------------------------
  lua_getglobal(g_L, "add");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function add does not exist in lua's global space : %s\n", lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  entry_position[0] = -1;
  lua_pushstring(g_L, args); // not pushing string as it causes a copy
  lua_pushlightuserdata(g_L, g_tests);
  lua_pushlightuserdata(g_L, entry_position);
  status = lua_pcall(g_L, 3, 0, 0);
  if (status != 0) {
    WHEREAMI; 
    fprintf(stderr, "calling function add failed: %s\n", lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
    if (entry_position[0] != -1) {
      status = free_test(entry_position[0]); cBYE(status);
    }
    go_BYE(-1);
  }
  // printf("changed value: %d\n", cdata[0]);
BYE:
  return status;
}
