/* 
gcc -g -Wall -std=gnu99 \
  -I../src/ \
  test_mysql.c \
  ../src/chk_db_conn.c \
  ../src/libs/libmysqlclient.so \
  -o test_mysql
*/
#include "ab_incs.h"
#include "macros.h"
#include "chk_db_conn.h"

CFG_TYPE g_cfg;
int
main(
    int argc,
    char **argv
    )
{
  int status= 0;
  const char *server = "127.0.0.1";
  const char *user = "root";
  const char *password = "";
  const char *database = "abdb2";
  int32_t port = 3306;

  status = internal_chk_db_conn(server, user, password, database, port);
  cBYE(status);
  fprintf(stderr, "SUCCESS\n");
BYE:
  return status;
}
