#!/bin/bash
set -e 

curl -s -XPOST --url "http://localhost:8000/PostProcPreds?Testing=true" -o _rslt
jq . _rslt
echo ""
echo $rslt
echo "Completed $0 in $PWD"
