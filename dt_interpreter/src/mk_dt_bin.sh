#!/bin/bash
set -e
if [ $# != 1 ]; then echo "Error. Usage is $0 <dt file>"; exit 1; fi 
make clean 1>/dev/null 2>&1
make       1>/dev/null 2>&1
source ../../src/to_source
# This is the decision tree as a CSV file 
#dt_csv_file=../../DT/spam/dt.csv
dt_csv_file=$1
test -f $dt_csv_file
# This is an optional test file 
testfile=cc_member_model_test_file
if [ "$testfile" != "" ]; then 
  test -f $testfile 
fi
# These are 3 binary files created by the process
dtfile=_dt.bin
rffile=_rf.bin
mdlfile=_mdl.bin
VG="valgrind --leak-check=full"
VG=""
$VG ./test_dt $dt_csv_file $dtfile $rffile $mdlfile $testfile 1>_out 2>&1
if [ "$VG" != "" ]; then 
  grep "ERROR SUMMARY: 0 errors" _out 1>/dev/null 2>&1
fi
echo "Successfully completed $0 in $PWD"
