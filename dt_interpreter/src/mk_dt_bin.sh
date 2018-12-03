#!/bin/bash
set -e
if [ $# != 2 ]; then echo 
  "Error. Usage is $0 <dt file> <random_forest|xgboost>"; exit 1; 
fi 
make clean 1>/dev/null 2>&1
make       1>/dev/null 2>&1
source ../../src/to_source
# This is the decision tree as a CSV file 
#dt_csv_file=../../DT/spam/dt.csv
dt_csv_file=$1
test -f $dt_csv_file
foresttype=$2
ok=0
if [ $foresttype == "random_forest" ]; then ok=1; fi
if [ $foresttype == "xgboost" ]; then ok=1; fi
if [ $ok = 0 ]; then 
  echo "foresttype must be random_forest or xgboost"; exit 1; 
fi
# This is an optional test file 
# testfile=cc_member_model_test_file
if [ "$testfile" != "" ]; then 
  test -f $testfile 
fi
# These are 3 binary files created by the process
dtfile=_dt.bin
rffile=_rf.bin
mdlfile=_mdl.bin
VG="valgrind --leak-check=full"
VG=""
echo ./test_dt $dt_csv_file $dtfile $rffile $mdlfile $foresttype $testfile 
$VG ./test_dt $dt_csv_file $dtfile $rffile $mdlfile $foresttype $testfile 1>_out 2>&1
if [ "$VG" != "" ]; then 
  grep "ERROR SUMMARY: 0 errors" _out 1>/dev/null 2>&1
fi
echo "Successfully completed $0 in $PWD"
