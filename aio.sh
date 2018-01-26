#!/bin/bash
set-env -e
sudo apt-get install gcc python python-pip -y
sudo pip install pystatsd
rm -rf bin
rm -f bin.tar.gz
mkdir bin
cd src
make clean
cd ../curl-7.51.0/
make clean
./configure
make
cd ../src
make
cp ab_httpd ../bin
cd ../
mkdir ./bin/libs
find ./ -name "*.so*" -exec cp {} ./bin/libs \;
find ./src -name "*.lua" -exec cp {} ./bin \;
tar -cvzf bin.tar.gz ./bin

