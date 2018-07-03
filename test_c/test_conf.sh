#!/bin/bash
set -e 
conf_file='../src/ab.conf.json'
test -f $conf_file
gcc -g -Wall -std=gnu99 \
  -I../src/ -I../jansson-2.10/src/ \
  ../src/read_conf_file.c ../src/auxil.c \
  ../src/mmap.c  test_read_conf_file.c  ../src/validate_config.c  \
  ../jansson-2.10/src/.libs/libjansson.so \
  -o test_conf
VG="valgrind --leak-check=full --show-leak-kinds=all"
$VG ./test_conf $conf_file

echo "Successfully completed $0 in $PWD"
