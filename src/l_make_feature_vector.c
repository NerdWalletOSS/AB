#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "l_make_feature_vector.h"

#ifdef XXXX
    char *str_feature_vector, // assume JSON, assume the config file is provided within JSON
    float *feature_vector, // assume not yet initialised to 0, so I'll do the dirty work
    int g_n_dt_feature_vector // length is supplied by l_get_num_features.c
#endif

int 
l_make_feature_vector(
    char *body, // TODO P3 give better name 
    bool is_debug 
    )
{
  int status = 0;
  lua_getglobal(g_L_DT, "make_feature_vector");
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua Function make_feature_vector undefined\n");
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  lua_pushstring(g_L_DT, body); 
  lua_pushlightuserdata(g_L_DT, g_dt_feature_vector);
  lua_pushnumber(g_L_DT, g_n_dt_feature_vector);
  lua_pushlightuserdata(g_L_DT, g_rslt);
  lua_pushnumber(g_L_DT, AB_MAX_LEN_RESULT);
  lua_pushboolean(g_L_DT, is_debug);
  status = lua_pcall(g_L_DT, 6, 0, 0);
  if ( status != 0 ) {
    fprintf(stderr, "calling function set_dt_feature_vector failed: %s\n", lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
BYE:
  return status;
}
