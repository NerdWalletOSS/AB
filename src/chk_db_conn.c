#include "ab_incs.h"
#include "chk_db_conn.h"

#include <mysql/mysql.h>
#include "macros.h"
#include "ab_incs.h"
#define LEN_QSTR 2048

extern CFG_TYPE g_cfg;

static int
load_lkp_tbl(
    MYSQL *conn,
    const char *tbl
    )
//</hdr>
{
  int status = 0;
  MYSQL_RES *rslt_set = NULL;
  char qstr[LEN_QSTR]; int nw;

  memset(qstr, '\0', LEN_QSTR);
  nw = snprintf(qstr, LEN_QSTR, "select id, name from %s ", tbl);
  if ( nw >= LEN_QSTR ) { go_BYE(-1); }

  status = mysql_query(conn, qstr); cBYE(status);
  rslt_set = mysql_store_result (conn);
  if ( rslt_set == NULL ) { go_BYE(-1); }
  int n  = mysql_num_rows(rslt_set); 
  if ( n == 0 ) { go_BYE(-1); }
  MYSQL_ROW row;
  int chk_n = 0;
  while ( (row = mysql_fetch_row(rslt_set)) != NULL ) { 
    // fetched a row 
    chk_n++;
  }
  if ( chk_n != n ) { go_BYE(-1); }
BYE:
  if ( rslt_set != NULL ) { mysql_free_result(rslt_set); }
  return status;
}

int
internal_chk_db_conn(
    const char *server,
    const char *user,
    const char *password,
    const char *database,
    int32_t port
    )
{
  int status = 0;
  void *vptr = NULL;
  MYSQL *conn = NULL;
  /* initialize connection handler */
  conn = mysql_init (NULL);
  if ( conn == NULL) { go_BYE(-1); }
  /* connect to server */
  vptr = mysql_real_connect (conn, server, user, 
      password, database, port, NULL, 0);
  if ( vptr == NULL ) { 
    fprintf (stderr, "mysql_real_connect() failed\n");
    if ( conn != NULL )  { mysql_close (conn); conn = NULL; }
    go_BYE(-1); 
  }
  status = load_lkp_tbl(conn, "admin"); cBYE(status);
BYE:
  /* disconnect from server */
  if ( conn != NULL ) { mysql_close (conn); conn = NULL; }
  mysql_library_end();
  return status;
}

int
chk_db_conn(
    void
    )

{
  int status = 0;
  status = internal_chk_db_conn(
      g_cfg.mysql.server, 
      g_cfg.mysql.user, 
      g_cfg.mysql.password, 
      g_cfg.mysql.database, 
      g_cfg.mysql.port
      );
  cBYE(status);
BYE:
  return status;
}
    
