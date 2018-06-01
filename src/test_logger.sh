#!/bin/bash
curl -XPOST http://logger.nerdwallet.biz/api/v1/abserver/ABLog --data '{ "abtest_id": 342534, "ramp" : "1", "uuid": "foo", "test": "foobar", "test_id": 123, "variant": "blue", "variant_id": 5, "time": 3426 }' --header "Content-Type: application/json"
# should return something like this. Look for data OK
# {"meta": {"local_request_id": "70bc062b-f169-4109-abc2-b6391f3ebf84",
# "request_id": null, "request_json_valid": true, "session_id": null,
# "originating_referrer": null, "pageview_id": null, "client_id": null,
# "originating_user_agent": null, "request_args_valid": null, "cookie_id": null,
# "originating_url": null}, "data": "OK"}
