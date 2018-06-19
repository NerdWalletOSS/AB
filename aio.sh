#!/bin/bash
usage(){
  echo "Usage: $0 [-h|-b|-c|-p] -- program to build AB and pack the bin and dependencies"
  echo "where:"
  echo "  -h  shows this message"
  echo "  -b  builds AB and places everything in the bin directory"
  echo "  -o  Same as -b but doesn't install packages build"
  echo "  -c  Cleans out all the binary files"
  echo "  -p  Builds tarballs for AB and abadmin in addition to building everything"
  echo "  -t  Installs testing utils"
  echo "  -r  Builds AB and runs tests"
  echo "  -x  Builds a tarball of all the php companents for AB admin"
  echo "  -s  Builds and starts all support systems"

  # echo "  -r  Builds and run AB with all other auxillary service"
  exit 1 ;
}

clean(){
  rm -rf bin
  rm -f bin.tar.gz
  make -C ./src clean
  make -C ./curl-7.51.0 clean
}

buildall(){
  sudo apt-get update
  sudo apt-get install gcc python python-pip cmake -y
  sudo pip install pystatsd
  clean
  install_deps_from_source
  install_librdkafka
  set -e
  build
}

install_librdkafka(){
  rm -rf ./librdkafka
  tar -xvzf librdkafka.tar.gz
  cd librdkafka/
  ./configure
  make
  cd ../
}

build(){
  rm -rf bin
  mkdir bin
  # cd ./curl-7.51.0/
  # ./configure
  # make
  # cd ../
  cd ./src
  make
  mkdir -p ../bin/libs
  cp ab_httpd ../bin
  cd ../
  find ./RTS -name "*.lua" -and -not -name "test_*.lua" -exec cp --parents \{\} ./bin/ \;
  find ./DT -name "*.lua" -and -not -name "test_*.lua" -exec cp --parents \{\} ./bin/ \;
  find ./lua -name "*.lua" -and -not -name "test_*.lua" -exec cp --parents \{\} ./bin/ \;

  find ./ -name "*.so*" -exec cp \{\} ./bin/libs/ \;
  set +e
  cd bin
  echo "cd libs" > to_source
  echo 'export LD_LIBRARY_PATH="`pwd`"' >> to_source
  echo "cd .." >> to_source
  echo 'export LUA_PATH="`pwd`/?.lua;;"' >> to_source
  cd -
}

clean_kafka_logs(){
  sudo rm -f /opt/kafka/logs/*
}

install_kafka(){
  sudo apt-get update
  sudo apt-get install default-jre -y
  sudo apt-get install zookeeperd zookeeper -y
  RES="`echo ruok | nc localhost 2181`"
  if [[ "$RES" != "imok" ]]
  then
    echo "Zookeeper is not working\n"
    exit 1
  fi
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
  /opt/kafka/bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 1 --partitions 1 --topic ab &
  kill -9 %2
  /opt/kafka/bin/kafka-topics.sh --list --zookeeper localhost:2181 | grep "ab"
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
    /opt/kafka/bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic test --from-beginning &
  fi

}

run_logger(){
  if [ "`ps auxwwf | grep "ab_logger" | grep -v "grep" | wc -l`" -ne 1 ]
  then
    cd bin/libs
    export LD_LIBRARY_PATH="`pwd`"
    cd -
    cd logger
    make
    ./ab_logger 8004 &
    cd ../
  fi
}


start_support_systems(){
  clean_kafka_logs
  run_kafka
  run_logger
}


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
  fi
}

install_apache(){
  which apache2
  RES="`echo $?`"
  if [ $RES -ne 0 ]
  then
    sudo apt-get install lua apache2 libapache2-mod-php -y
    cd ../
    sudo ln -s AB/ /var/www/html/AB
    mkdir -p /opt/abadmin/
    sudo chown `whoami`:`whoami` /opt/abadmin
    echo -e '{\n  "dbhost" : "127.0.0.1",\n  "dbname" : "abdb",\n  "dbuser" : "root",\n  "dbpass" : "",\n  "webapp_server" : "localhost",\n  "webapp_port" : "8080",\n  "ab_rts_server" : "",\n  "ab_rts_port" : "8000",\n  "ab_log_server" : "localhost",\n  "ab_log_port" : "8004",\n  "rts_finder_server" : "localhost",\n  "rts_finder_port" : "8020",\n  check_url_reachable" : "false",\n  "default_landing_page" : "http://www.nerdwallet.com"\n}' > /opt/abadmin/db.json
    sudo sed  -i 's/Listen 80/Listen 8080/g' /etc/apache2/ports.conf
    sudo /etc/init.d/apache2 restart
    cd -
  fi
}

run_lua_tests(){
  sudo luarocks install busted Lua-cURL luasec
  cd ./test_rts_lua/
  busted ./test_*.lua --lua=luajit -c  &>/dev/null
  STATUS="`echo $?`"
  if [ $STATUS -ne 0 ]
  then
    echo "ERROR RTS lua tests failed"
    exit 1
  fi
}

install_deps_from_source() {
  my_print "Installing luajit from source"
  #wget http://luajit.org/download/LuaJIT-2.0.4.tar.gz
  wget http://luajit.org/download/LuaJIT-2.1.0-beta3.tar.gz
  #tar -xvf LuaJIT-2.0.4.tar.gz
  tar -xvf LuaJIT-2.1.0-beta3.tar.gz
  #cd LuaJIT-2.0.4/
  cd LuaJIT-2.1.0-beta3/
  sed -i '114s/#//' src/Makefile # to enable gc64
  make TARGET_FLAGS=-pthread
  sudo make install
  cd /usr/local/bin
  sudo ln -sf luajit-2.1.0-beta3 /usr/local/bin/L
  sudo ln -sf luajit-2.1.0-beta3 /usr/local/bin/luajit

  cd -
  cd ../
  rm -rf LuaJIT-2.1.0-beta3
  sudo apt-get install luarocks
  sudo luarocks install busted Lua-cURL luasec
}

install_travis(){
  build

}


while getopts "sbochptx" opt;
do
  case $opt in
    h)
      usage
      ;;
    p)
      # echo "-f was triggered, Parameter: $OPTARG" >&2
      build
      tar -cvzf bin.tar.gz ./bin
      tar -cvzf php.tar.gz php app
      exit 0
      ;;
    s)
      start_support_systems
      exit 0
      ;;
    b)
      buildall
      exit 0
      ;;
    o)
      build
      exit 0
      ;;
    c)
      clean
      exit 0
      ;;
    t)
      buildall
      install_apache
      install_mysql
      install_kafka
      run_lua_tests
      echo "All Tests succeeded"
      exit 0
      ;;
    x)
      tar -cvzf php.tar.gz php app
      exit 0
      ;;
    \?)
      echo "Invalid option: -$OPTARG" 1>&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." 1>&2
      exit 1
      ;;
  esac
done
usage

# sudo /opt/kafka/bin/kafka-server-start.sh /opt/kafka/config/server.properties
