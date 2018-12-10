#!/bin/bash
set -e 
curl --url "localhost:8000/GetNumFeatures"
echo ""
curl --url "localhost:8000/PostProcPreds?Testing=true"
echo ""
curl --url "localhost:8000/GetConfig"
echo ""
curl --url "localhost:8000/Halt"
echo ""
echo "Smoke test passed"
