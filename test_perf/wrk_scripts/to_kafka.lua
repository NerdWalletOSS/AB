-- example HTTP POST script which demonstrates setting the
-- HTTP method, body, and adding a header

wrk.method = "POST"
wrk.body   = "foo=bar&baz=quux"
wrk.headers["Content-Type"] = " application/json"

wrk.body = '{ "abtest_id": 342534, "ramp" : "1", "uuid": "foo", "test": "foobar", "test_id": 123, "variant": "blue", "variant_id": 5, "time": 3426 }' 
