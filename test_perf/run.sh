#!/bin/bash
set -e 
S=localhost
P=8000
num_tests=32
num_iters=32
num_users=32
curl --url "$S:$P/Ignore"
curl --url "$S:$P/Restart"
./add_tests $S $P $num_tests _test_file
./pblaster $S $P _test_file $num_iters $num_users
# curl --url "$S:$P/Halt"
echo ALL DONE
