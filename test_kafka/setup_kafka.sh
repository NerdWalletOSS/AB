#!/bin/bash
usage(){
	echo "Usage: $0 [-h|-i|-r|-s] -- program to manage kafka"
	echo "where:"
	echo "  -h  shows this message"
	echo "  -i  Installs kafka"
	echo "  -r  Runs kafka"
	echo "  -s  Shuts down kafka and cleans logs"

	# echo "  -r  Builds and run AB with all other auxillary service"
	exit 1 ;
}

install_kafka(){
	sudo apt-get update
	# sudo apt-get install default-jre -y
	sudo apt-get install openjdk-8-jdk -y
	sudo apt-get install zookeeperd zookeeper -y
	# RES="`echo ruok | nc localhost 2181`"
	# if [[ "$RES" != "imok" ]]
	# then
	# 	echo "Zookeeper is not working\n"
	# `exit 1
	# `i
	sudo adduser --system --no-create-home --disabled-password --disabled-login kafka
	wget "http://www-eu.apache.org/dist/kafka/1.0.1/kafka_2.12-1.0.1.tgz"
	curl http://kafka.apache.org/KEYS | gpg --import
	wget https://dist.apache.org/repos/dist/release/kafka/1.0.1/kafka_2.12-1.0.1.tgz.asc
	gpg --verify kafka_2.12-1.0.1.tgz.asc kafka_2.12-1.0.1.tgz
	sudo mkdir /opt/kafka
	sudo tar -xvzf kafka_2.12-1.0.1.tgz --directory /opt/kafka --strip-components 1
	rm -rf kafka_2.12-1.0.1.tgz kafka_2.12-1.0.1.tgz.asc
	sudo mkdir /var/lib/kafka
	sudo mkdir /var/lib/kafka/data
	sudo chown -R kafka:nogroup /opt/kafka
	sudo chown -R kafka:nogroup /var/lib/kafka
	sudo /opt/kafka/bin/kafka-server-start.sh /opt/kafka/config/server.properties &
	sudo /opt/kafka/bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 1 --partitions 1 --topic ab &
	sudo kill -9 %2
	sudo /opt/kafka/bin/kafka-topics.sh --list --zookeeper localhost:2181 | grep "ab"
	RES="`echo $?`"
	if [[ "$RES" != "0" ]]
	then
		echo "Unable to get registered topic ab"
		exit 1
	fi
	kill -9 %1
}

run_kafka(){
	sudo ls /opt/kafka/bin/kafka-server-start.sh
	RES="`echo $?`"
	if [ $RES -ne 0 ]
	then
		install_kafka
	fi
	if [ "`ps auxwwf | grep "kafka-server-start" | grep -v "grep" | wc -l`" -ne 1 ]
	then
		sudo /opt/kafka/bin/kafka-server-start.sh /opt/kafka/config/server.properties &>/dev/null &
		# /opt/kafka/bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic test --from-beginning &
	fi

}

stop_kafka(){
	sudo /opt/kafka/bin/kafka-server-stop.sh
	ps auxwwf | grep "kafka" | awk '{print $2}'  | sudo xargs kill -9
  sudo rm -rf /opt/kafka/logs/*
}

while getopts "hirs" opt;
do
	case $opt in
		h)
			usage
			;;
		i)
			install_kafka
			exit 0
			;;
			r)
			run_kafka
			exit 0
			;;
			s)
			stop_kafka
			exit 0
			;;
	esac
done
usage
