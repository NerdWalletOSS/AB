#!/bin/bash
set env -e
cd ../
bash aio.sh -b
cd ../bin/libs
export LD_LIBRARY_PATH="`pwd`"
cd -
cd ../bin
./ab_httpd &
cd - 
curl -H "Content-Type: application/json" -X POST --url "http://localhost:8000/addTest" --data @../test_webapp/sample1.json
lua test_percentages.lua
kill -9 %1
