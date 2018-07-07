To run the server 
python ./fake_session_service.py -p 8024 -s1:10 &
This starts the server on port 8024 and has a uniformly distributed sleep time
between 1 and 10 milli seconds

To load new data run
curl --url "localhost:8024/load" \
  -H "Content-Type: application/json" \
  --data @fake_data.json

This would load the file fake_data.json into the server. 
There are some basic checks to what are valid keys are this point

To query the server run
curl "http://localhost:8024/session/v1/123/?fields=cookie_id,device"

This would request the fields cookie_id and device for the request sent. Right
now I just pick up everything from the database (including the cookie_id). Do
you want me to echo back the cookie_id used in the request?

To check the state run
curl http://localhost:8024/state

