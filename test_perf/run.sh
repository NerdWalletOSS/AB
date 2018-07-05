#!/bin/bash
set -e 
S=localhost
P=8000
num_tests=128
num_iters=64
num_users=64
num_threads=32
rm -f pblaster sblaster
make clean
make
curl --url "$S:$P/Ignore"
curl --url "$S:$P/Restart"
./add_tests $S $P $num_tests _test_file
# ./sblaster $S $P _test_file $num_iters $num_users 1
./pblaster $S $P _test_file $num_iters $num_users $num_threads
# curl --url "$S:$P/Halt"
echo ALL DONE
