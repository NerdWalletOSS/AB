#/bin/bash
set -e 
sudo iptables -I INPUT -j ACCEPT
sudo /opt/kafka/bin/kafka-server-start.sh /opt/kafka/config/server.properties &
