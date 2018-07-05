sudo pip install boto
sudo pip install flask

The -f option allows one to specify a config file (JSON format) for the 
sever to use. The default is /opt/abadmin/db.json.

The -T option is used for testing. In this case, instead of hitting the 
AWS Load Balancer to figure out where the IP address/port of the RTS server,
it will use the config file.



curl --url "localhost:8020/DescribeInstances"

Response -> 
{
"server": ["172.20.101.30", "172.20.102.41", "172.20.103.225”],
"port": [ 8000,8000, 8000 ] 
}

The port is taken from /opt/abadmin/db.json
If you want to force the json file, use the -f option e.g., 
$python rts_server.py -f ./db.json
=========================================================

For local testing, 
python rts_server.py -T localhost:8000  &
python rts_server.py -T localhost:8000 localhost:8004 localhost:8008 

Request -> curl http://0.0.0.0:8020/DescribeInstances
{
"server": ["localhost", "localhost", "localhost"], 
"port": [ 8000,8004, 8008 ] 
}

