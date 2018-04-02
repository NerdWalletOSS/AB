#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "add_test.h"

// Ideally returns the number of features.
// If screws up, returns -1
int
l_get_num_features(
    const char *model_nm
    )
{
  int status = 0;
  lua_getglobal(g_L, "get_num_features"); // what is g_L?
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function get_num_features does not exist in lua's global space\n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  lua_pushstring(g_L, model_nm); // not pushing string as it causes a copy
  status = lua_pcall(g_L, 1, 1, 0);
  if (status != 0) {
    fprintf(stderr, "calling function get_num_features failed: %s\n", lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  int n_feats = lua_tonumber(g_L, -1);
  // to Indrajeet: must I do more about the stack?
  printf("Number of features: %d\n", n_feats);
  return n_feats;
BYE:
  return -1;
}
