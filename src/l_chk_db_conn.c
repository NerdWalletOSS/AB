#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "l_chk_db_conn.h"

int
l_chk_db_conn(
    void
    )
{
  int status = 0;
  if ( g_L == NULL ) { go_BYE(-1); }
  lua_getglobal(g_L, "check_db_conn");
  if ( !lua_isfunction(g_L, -1)) {
    fprintf(stderr, "Function add does not exist in lua's global space\n");
    lua_pop(g_L, 1);
    go_BYE(-1);
  }

  status = lua_pcall(g_L, 0, 0, 0);
  if (status != 0) {
    WHEREAMI;
    fprintf(stderr, "calling function check_db_conn failed: %s\n", lua_tostring(g_L, -1));
    sprintf(g_err, "{ \"error\": \"%s\"}",lua_tostring(g_L, -1));
    lua_pop(g_L, 1);
    go_BYE(-1);
  }

BYE:
  return status;
}
