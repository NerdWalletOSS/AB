#!/bin/bash
if [ ! -f /opt/kafka/bin/kafka-server-start.sh ]; then
  echo "Please install kafka"
  exit 1
fi
echo "Starting kafka"
sudo /opt/kafka/bin/kafka-server-start.sh /opt/kafka/config/server.properties &
