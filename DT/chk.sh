#!/bin/bash
set -e
if [ $# != 1 ]; then echo "Error. Usage is $0 <directory>"; fi 
dir=$1
test -d $dir
cd $dir
test -f dt.csv
luajit dt_feature.lua
luajit mdl_map.lua
luajit mdl_meta.lua
luajit generate_features.lua
test -f sample_input.json
jq . sample_input.json
test -f sample_output.json
jq . sample_output.json
#------------------------
cwd=$PWD
echo "XXXX $cwd"
pushd .
cd ../../dt_interpreter/src/
echo bash mk_dt_bin.sh $cwd/dt.csv
bash mk_dt_bin.sh $cwd/dt.csv
test -f _dt.bin;  mv _dt.bin  $cwd
test -f _rf.bin;  mv _rf.bin  $cwd
test -f _mdl.bin; mv _mdl.bin $cwd
popd
exit
#------------------------
cd test/
pwd
## Need to run all the scripts here
ls test*.lua > _x
while read line 
do
  echo "Testing $line"
  busted $line 1>_out
  grep "0 errors" _out 1>/dev/null 2>&1
done < _x
rm _*
echo "Completed $0 in $PWD"
