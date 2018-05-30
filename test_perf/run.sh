#!/bin/bash
set -e 
S=localhost
P=8000
num_tests=16
num_iters=65536
num_users=65536
curl --url "$S:$P/Ignore"
curl --url "$S:$P/Restart"
./add_tests $S $P $num_tests _test_file
echo ./sblaster $S $P _test_file $num_iters $num_users
echo ALL DONE
