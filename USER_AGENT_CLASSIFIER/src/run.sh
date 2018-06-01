#/bin/bash
set -e 
make clean
make
test -d ../data/
mfile=../data/model_coefficients.csv
cfile=../data/category_intercept.csv
tfile=../data/sample_user_agent.csv
test -f $mfile
test -f $cfile
VG=""
VG="valgrind --leak-check=full "
$VG ./test_ua $mfile $cfile $tfile 1>_out 2>&1
grep "0 errors" _out  1>/dev/null 2>&1
echo "Successfully completed $0 in $PWD"
