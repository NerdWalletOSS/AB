#!/bin/bash
set -e
#-- MAKE CHANGES BELOW THIS LINE
port=8080 
dir="$PWD"/TGZ_FILES/
sample_conf=doc/httpd.conf
#-- DO NOT CHANGE BELOW THIS LINE
test -f $sample_conf
conf_file="$PWD"/"$sample_conf"
test -d $dir
pushd .
cd $dir
cwd=$PWD
test -f php-7.2.4.tar.bz2
test -f httpd-2.4.33.tar.bz2
test -f apr-1.6.3.tar.gz
test -f apr-util-1.6.1.tar.gz

sudo apt-get install libpcre3-dev
sudo apt-get install libssl-dev
sudo apt-get install bzip2
sudo apt-get install libexpat1-dev
sudo apt-get install libxml2-dev
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install libmysqlclient-dev

bunzip2 httpd-2.4.33.tar.bz2 
tar -xvf httpd-2.4.33.tar 

tar -zxf apr-util-1.6.1.tar.gz

tar -zxf apr-1.6.3.tar.gz

cd httpd-2.4.33/
mv ../apr-util-1.6.1 ./srclib/apr-util
mv ../apr-1.6.3      ./srclib/apr

options=" --enable-so \
--prefix=$HOME/local/ \
--with-included-apr \
--with-libxml2 \
--enable-proxy \
--enable-curl \
--enable-headers \
--enable-cache \
--enable-ssl"
#-- have taken out enable-lua
./configure $options
make
make install
cd $cwd

#=================================
bunzip2 php-7.2.4.tar.bz2 
tar -xvf php-7.2.4.tar 
cd php-7.2.4/

options=" --prefix=$HOME/local/ \
--with-apxs2=$HOME/local/bin/apxs \
--with-libdir=lib64 \
--with-pdo-mysql \
--with-zlib \
--with-openssl \
--enable-pcntl \
--with-curl \
--enable-mbstring "
./configure $options
make
make install

libtool --finish ./libs
# copy config file to correct location
cp $conf_file $HOME/local/httpd.conf
# Now make changes to conf file
# Change ServerName
# Change Listen 8080
echo "SUCCESS for $0 in $PWD"

