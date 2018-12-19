#!/bin/bash
set -e
# sample call: bash static_chk.sh spam random_forest
if [ $# != 2 ]; then 
  echo "Error. Usage is $0 <directory> <random_forest|xgboost>"; fi 
dir=$1
foresttype=$2
test -d $dir
cd $dir
test -f dt.csv
luajit dt_features.lua
luajit mdl_map.lua
luajit mdl_meta.lua
test -f sample_input.json
jq . sample_input.json
test -f sample_output.json
jq . sample_output.json
#------------------------
cwd=$PWD
echo "XXXX $cwd"
pushd .
cd ../../dt_interpreter/src/
echo bash mk_dt_bin.sh $cwd/dt.csv "$foresttype"
bash mk_dt_bin.sh $cwd/dt.csv "$foresttype"
test -f _dt.bin;  mv _dt.bin  $cwd
test -f _rf.bin;  mv _rf.bin  $cwd
test -f _mdl.bin; mv _mdl.bin $cwd
popd
echo SUCCESS
