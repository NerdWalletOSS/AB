#!/bin/bash
set -e
iter=1
niter=1000
while [ $iter -le $niter ]; do
curl -XPOST http://localhost:8000/ToKafka --data '{ "abtest_id": 342534, "ramp" : "1", "uuid": "foo", "test": "foobar", "test_id": 123, "variant": "blue", "variant_id": 5, "time": 3426 }' --header "Content-Type: application/json"
  echo "Done $iter"
  iter=`expr $iter + 1`

done
curl --url "localhost:8000/Halt"
