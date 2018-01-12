#!?bin/bash
set -e
WARN="  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic "
gcc -g -DLOCAL_DEPLOY ${WARN} cli_ua_to_dev.c  \
  -I../ -I../../hhash/  \
  -I../../statsd-c-client-master/ \
  -I../../curl-7.51.0/include/ \
  ../spooky_hash.c  \
  ../load_device.c  \
  ../ua_to_device.c  \
  ../url.c  \
  ../load_ua_to_dev_map.c  \
  ../auxil.c \
  ../mmap.c 
devfile=/opt/ab/device.csv
mapfile=/opt/ab/ua_to_dev.bin
while read ua 
do 
  echo "Testing $ua"
  ./a.out $devfile $mapfile "$ua"
done < sample_user_agent.csv
echo "Successfully completed $0 in $PWD"
