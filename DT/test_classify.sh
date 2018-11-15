#!/bin/bash
set -e 
if [ $# != 1 ]; then echo "ERROR. Usage is $0 <directory"; fi
dir=$1
test -d $dir
infile=$dir/sample_input.json
test -f $infile
jq . $infile 1>/dev/null 2>&1

outfile=$dir/sample_output.json
test -f $outfile
jq . $outfile 1>/dev/null 2>&1

body=`cat $infile`

curl -s -XPOST "http://localhost:8000/Classify" \
  --data "$body" \
  --header "Content-Type: application/json" -o _out.json
jq . _rslt 1>/dev/null 2>&1
echo ""
luajit lua/compare_predictions.lua _out.json $outfile
echo $rslt
echo "Completed $0 in $PWD"
