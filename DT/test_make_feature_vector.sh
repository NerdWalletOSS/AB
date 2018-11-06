#!/bin/bash
set -e 
if [ $# != 1 ]; then echo "ERROR. Usage is $0 <directory"; fi
dir=$1
test -d $dir
filename=$dir/sample_input.json
test -f $filename
body=`cat $filename`

curl -s -XPOST http://localhost:8000/MakeFeatureVector \
  --data "$body" \
  --header "Content-Type: application/json" 
echo "$rslt"
echo "Completed $0 in $PWD"
