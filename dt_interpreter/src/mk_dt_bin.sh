#!/bin/bash
set -e
if [ $# != 1 ]; then echo "Error. Usage is $0 <dt file>"; exit 1; fi 
make clean 1>/dev/null 2>&1
make       1>/dev/null 2>&1
# This is the decision tree as a CSV file 
#dt_csv_file=../../DT/spam/dt.csv
dt_csv_file=$1
test -f $dt_csv_file
# This is an optional test file 
# testfile=../data/spam_data/test_1_10.csv
# test -f $testfile
# These are 3 binary files created by the process
dtfile=_dt.bin
rffile=_rf.bin
mdlfile=_mdl.bin
VG=""
VG="valgrind --leak-check=full"
$VG ./test_dt $dt_csv_file $dtfile $rffile $mdlfile $testfile 1>_out 2>&1
grep "ERROR SUMMARY: 0 errors" _out 1>/dev/null 2>&1
echo "Successfully completed $0 in $PWD"
