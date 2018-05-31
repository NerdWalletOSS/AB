#!/bin/bash
set -e 

curl -XPOST http://localhost:8000/PostProcPreds 
echo ""
echo "Completed $0 in $PWD"
