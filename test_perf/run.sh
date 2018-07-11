#!/bin/bash
set -e 
S=localhost
P=8000
# S="172.30.103.93" # for BFM 
num_tests=128
num_iters=128
num_users=128
num_threads=16
rm -f pblaster sblaster
make clean
make
curl --url "$S:$P/Ignore"
curl --url "$S:$P/Restart"
./add_tests $S $P $num_tests _test_file
./sblaster $S $P _test_file $num_iters $num_users 1
# echo PREMATURE; exit
# ./pblaster $S $P _test_file $num_iters $num_users $num_threads
# curl --url "$S:$P/Halt"
echo ALL DONE
