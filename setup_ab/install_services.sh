#!/bin/bash
install_mysql(){
  which mysql
  RES="`echo $?`"
  if [ $RES -ne 0 ]
  then
    sudo apt-get update
    sudo debconf-set-selections <<< 'mysql-server mysql-server/root_password
    password x'
    sudo debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password x'
    sudo apt-get install mysql-server -y
    mysql -uroot -px -e "SET PASSWORD FOR root@localhost=PASSWORD('');"
  fi
}


install_statsd(){
  audo apt-get install ngrep
  pip install pystatsd
  python -c 'import pystatsd; pystatsd.Server().serve()' &

}

install_logger(){
  cd ../logger
  make
  ./ab_logger 8004 &
  cd -
}

set -e
cd ../
bash aio.sh -b
cd -

install_statsd
install_mysql
install_logger
# install_apache
# install_php
