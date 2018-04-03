#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "make_feature_vector.h"

int 
l_set_dt_feature_vector(
    char *str_feature_vector, // assume JSON, assume the config file is provided within JSON
    float *feature_vector, // assume not yet initialised to 0, so I'll do the dirty work
    int g_n_dt_feature_vector // length is supplied by l_get_num_features.c
    )
{
  int status = 0;
  lua_getglobal(g_L, "set_dt_feature_vector");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function set_dt_feature_vector does not exist in lua's global space\n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
  lua_pushstring(g_L, str_feature_vector);
  lua_pushlightuserdata(g_L, feature_vector);
  lua_pushnumber(g_L, g_n_dt_feature_vector);
  status = lua_pcall(g_L, 3, 0, 0);
  if (status != 0) {
    fprintf(stderr, "calling function set_dt_feature_vector failed: %s\n", lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
    go_BYE(-1);
  }
BYE:
  return status;
}
