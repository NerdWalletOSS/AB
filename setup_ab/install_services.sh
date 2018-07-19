#!/bin/bash
install_mysql(){
  which mysql
  RES="`echo $?`"
  if [ $RES -ne 0 ]
  then
    sudo apt-get update
  
		sudo debconf-set-selections <<< 'mysql-server mysql-server/root_password password x'
		sudo debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password x'


    sudo apt-get install mysql-server -y
    mysql -uroot -px -e "SET PASSWORD FOR root@localhost=PASSWORD('');"
    mysql -uroot < ../sql/create_table.sql
  fi
}


install_statsd(){
  sudo apt-get install ngrep -y
  cd ../
  bash aio.sh -t
  cd -
  sudo pip install pystatsd
  python -c 'import pystatsd; pystatsd.Server().serve()' &

}

install_logger(){
  cd ../logger
  make
  ./ab_logger 8004 &
  cd -
}

install_kafka(){
  cd ../test_kafka
  bash ./setup_kafka.sh -i
  bash ./setup_kafka.sh -r
  cd -
}

install_apache_php(){
  cd ../
  bash install.sh
  cd -
}

cd ../
bash aio.sh -b
cd -

install_statsd
install_mysql
install_logger
install_kafka
install_apache_php
