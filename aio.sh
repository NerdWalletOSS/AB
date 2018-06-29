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
  echo "  -x  Builds a tarball of all the php companents for AB admin"

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
  sudo apt-get install gcc cmake g++ -y
  sudo pip install pystatsd
  clean
  install_test_deps
  set -e
  build
}

build(){
  rm -rf bin
  mkdir bin
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

install_test_deps() {
  my_print "Installing luajit from source"
  tar -xvf LuaJIT-2.1.0-beta3.tar.gz
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
  sudo apt-get install luarocks python python-pip cmake -y
  sudo luarocks install busted Lua-cURL luasec
}


while getopts "bochptx" opt;
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
      install_test_deps
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
