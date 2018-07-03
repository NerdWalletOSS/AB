#!/bin/bash
set -e 
S=localhost
P=8000
num_tests=128
num_iters=64
num_users=64
curl --url "$S:$P/Ignore"
curl --url "$S:$P/Restart"
./add_tests $S $P $num_tests _test_file
./pblaster $S $P _test_file $num_iters $num_users
# curl --url "$S:$P/Halt"
echo ALL DONE
