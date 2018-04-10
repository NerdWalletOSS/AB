#!/bin/bash
usage(){
  echo "Usage: $0 [-h|-b|-c|-p] -- program to build AB and pack the bin and dependencies"
  echo "where:"
  echo "  -h  shows this message"
  echo "  -b  builds AB and places everything in the bin directory"
  echo "  -o  Same as -b but doesn't install packages build"
  echo "  -c  Cleans out all the binary files"
  echo "  -p  Builds a tarball in addition to building everything"
  echo "  -t  Builds AB and runs tests"
  exit 1 ;
}

clean(){
  rm -rf bin
  rm -f bin.tar.gz
  make -C ./src clean
  make -C ./curl-7.51.0 clean
}

buildall(){
  sudo apt-get install gcc python python-pip cmake -y
  sudo pip install pystatsd
  clean
  set -e
  build
}

build(){
  rm -rf bin
  mkdir bin
  cd ./curl-7.51.0/
  ./configure
  make
  cd ../
  mkdir ./bin/libs
  cd ./src
  make
  cp ab_httpd ../bin
  find ../ -name "*.so*" -exec cp {} ../bin/libs \;
  find ./ -name "*.lua" -exec cp --parents {} ../bin \;
  cd ../
  set +e
}

install_apache(){
  which apache2
  RES="`echo $?`"
  if [ $RES -eq 1 ]
  then
    sudo apt-get install lua apache2 -y
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
  if [ $STATUS -eq 1 ]
  then
    echo "ERROR RTS lua tests failed"
    exit 1
  fi
}

while getopts "bochpt" opt;
do
  case $opt in
    h)
      usage
      ;;
    p)
      # echo "-f was triggered, Parameter: $OPTARG" >&2
      build
      tar -cvzf bin.tar.gz ./bin
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
      run_lua_tests
      echo "All Tests succeeded"
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
