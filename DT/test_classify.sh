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

luajit test_classify.lua $infile $outfile
echo SUCCESS
