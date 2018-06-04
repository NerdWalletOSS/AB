#!/bin/bash
# This is how you create your own session
cookie=ramesh1
# following ua is classified as Mobile
ua="Mozilla/5.0 (Linux; Android 4.3; HTC One Build/JSS15J) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.72 Mobile Safari/537.36"
# the way to set paid is to have a url with this form:
# example123?utm_campaign=paid`
curl -v --url "logger/session/v1/?caller_client_id=abcli" \
  -X POST \
  -H "Content-type: application/json" \
  -d "{\"url\" : \"xyz?utm_campaign=paid\", \"user_agent\": \"$ua\", \"cookie_id\": \"$cookie\"}"

# to verify that it took
curl --url "logger:80/session/v1/$cookie?caller_client_id=abrts"

