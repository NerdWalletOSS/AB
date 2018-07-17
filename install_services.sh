#!/bin/bash

install_mysql(){
  which mysql
  RES="`echo $?`"
  if [ $RES -ne 0 ]
  then
    sudo apt-get update
    sudo debconf-set-selections <<< 'mysql-server mysql-server/root_password
password x'
    sudo debconf-set-selections <<< 'mysql-server
mysql-server/root_password_again password x'
    sudo apt-get install mysql-server -y
    mysql -uroot -px -e "SET PASSWORD FOR root@localhost=PASSWORD('');"
  fi
}


install_statsd(){
sudo apt-get install python-dev libcairo2-dev libffi-dev build-essential
export PYTHONPATH="/opt/graphite/lib/:/opt/graphite/webapp/"
sudo pip install --no-binary=:all: https://github.com/graphite-project/whisper/tarball/master
sudo pip install --no-binary=:all: https://github.com/graphite-project/carbon/tarball/master
sudo pip install --no-binary=:all: https://github.com/graphite-project/graphite-web/tarball/master
sudo pip install pystatsd

}




# install_apache
# install_php
