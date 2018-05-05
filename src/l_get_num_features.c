#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "l_get_num_features.h"

extern CFG_TYPE g_cfg;
// Ideally returns the number of features.
// If screws up, returns -1
int
l_get_num_features(
    int *ptr_num_features
    )
{
  int status = 0;
  char *luafile = NULL; int buflen = 0;
  if ( *g_cfg.dt_dir == '\0' ) { go_BYE(-1); }
  buflen = strlen(g_cfg.dt_dir) + strlen("get_num_features") + 4;
  luafile = malloc(buflen); return_if_malloc_failed(luafile); 
  memset(luafile, '\0', buflen); 
  sprintf(luafile, "%s/get_num_features", g_cfg.dt_dir);
  *ptr_num_features = 0;
  lua_getglobal(g_L_DT, luafile);
  if ( !lua_isfunction(g_L_DT, -1)) {
    fprintf(stderr, "Function get_num_features does not exist in %s\n", g_cfg.dt_dir);
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  status = lua_pcall(g_L_DT, 0, 1, 0);
  if (status != 0) {
    fprintf(stderr, "calling function get_num_features in DT failed: %s\n", lua_tostring(g_L_DT, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L_DT, -1));
    lua_pop(g_L_DT, 1);
    go_BYE(-1);
  }
  int num_features = lua_tonumber(g_L_DT, -1);
  lua_pop(g_L_DT, 1);
  if ( num_features <= 0 ) { go_BYE(-1); }
  *ptr_num_features = num_features;
BYE:
  free_if_non_null(luafile);
  return status;
}
