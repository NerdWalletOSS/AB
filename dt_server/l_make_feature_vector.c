#include "dt_incs.h"
#include <lua.h>
#include <jansson.h>
#include "auxil.h"
#include "l_make_feature_vector.h"
extern char g_err[DT_ERR_MSG_LEN+1]; // For C: ab_process_req()
extern DT_INTERPRETER_TYPE *g_interp;
extern lua_State *g_L_DT;

int 
l_make_feature_vector(
    const char * const fvec_as_json
    )
{
  int status = 0;
  const char *const lua_fn = "make_feature_vector";
  lua_getglobal(g_L_DT, lua_fn);
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Lua Function %s undefined\n", lua_fn);
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  lua_pushstring(g_L_DT, fvec_as_json); 
  lua_pushlightuserdata(g_L_DT, g_interp->dt_feature_vector);
  lua_pushnumber(g_L_DT, g_interp->n_dt_feature_vector);
  status = lua_pcall(g_L_DT, 3, 1, 0);
  if ( status != 0 ) {
    fprintf(stderr, "calling function %s failed: %s\n", 
        lua_fn, lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    fprintf(stderr, "%s\n", fvec_as_json);
    lua_pop(g_L_DT, 1); go_BYE(-1);
  }
  if (!lua_isboolean(g_L_DT, -1)) {
    fprintf(stderr, "%s: return 1 must be a boolean\n", __func__); go_BYE(-1); 
  }
  bool l_status = lua_toboolean(g_L_DT, -1);
  if ( l_status != true ) { go_BYE(-1); }
BYE:
  return status;
}

extern char g_dt_features[DT_MAX_NUM_FEATURES][DT_MAX_LEN_FEATURE+1]; 
extern int g_n_dt_features;
int 
alt_make_feature_vector(
    const char * const fvec_as_json
    )
{
  int status = 0;
  json_t *root = NULL;
  json_error_t error;
  mcr_chk_non_null(fvec_as_json);
  root = json_loads(fvec_as_json, 0, &error);
  if ( root == NULL ) { go_BYE(-1); }

  for ( int i = 0; i < g_n_dt_features; i++ ) { 
    json_t *feature_handle = json_object_get(root, g_dt_features[i]);
    if ( feature_handle == NULL ) { go_BYE(-1); }
    double feature_val = json_number_value(feature_handle);
    g_interp->dt_feature_vector[i] = feature_val;
    // fprintf(stderr, "%d:%s -> %lf \n", i, g_dt_features[i], feature_val);
  }

BYE:
  return status;
}
