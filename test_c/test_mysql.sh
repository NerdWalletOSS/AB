#!/bin/bash
set -e 
gcc -g -Wall -std=gnu99 \
  -I../src/ \
  test_mysql.c \
  ../src/chk_db_conn.c \
  ../src/libs/libmysqlclient.so \
  -o test_mysql
VG="valgrind --leak-check=full --show-leak-kinds=all"
$VG ./test_mysql $conf_file

echo "Successfully completed $0 in $PWD"
