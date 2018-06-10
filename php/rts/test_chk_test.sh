#!/bin/bash
set -e
url="http://localhost:8080/AB/php/endpoints/endpoint_chk_test.php"
body=' { "TestName" : "Test1", "TestType" ; "XYTest" }'
curl -XPOST --url "$url" --data "$body"
echo DONE
url="http://localhost:8080/AB/php/endpoints/endpoint_chk_tests.php"
curl -XPOST --url "$url" 
echo ALL DONE
