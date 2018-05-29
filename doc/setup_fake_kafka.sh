#/bin/bash
set -e 
sudo apt-get update -y
sudo apt-get upgrade -y
sudo add-apt-repository -y ppa:webupd8team/java
sudo apt-get update
sudo apt-get install oracle-java8-installer -y
sudo apt-get install zookeeperd
netstat -ant | grep :2181

wget "http://www-eu.apache.org/dist/kafka/1.0.1/kafka_2.12-1.0.1.tgz"
sudo mkdir -p /opt/kafka
sudo tar -xvzf kafka_2.12-1.0.1.tgz --directory /opt/kafka --strip-components 1
rm -rf kafka_2.12-1.0.1.tgz kafka_2.12-1.0.1.tgz.asc
sudo mkdir -p /var/lib/kafka
sudo mkdir -p /var/lib/kafka/data
sudo chown -R kafka:nogroup /opt/kafka
# INDRAJEET TODO Check is user kafka exists
sudo adduser --system --no-create-home --disabled-password --disabled-login kafka
sudo chown -R kafka:nogroup /opt/kafka
sudo chown -R kafka:nogroup /var/lib/kafka
# sudo /opt/kafka/bin/kafka-server-start.sh /opt/kafka/config/server.properties &
sudo iptables -I INPUT -j ACCEPT
