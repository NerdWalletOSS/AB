#!?bin/bash
set -e
WARN=" -g -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic "
gcc -g -DLOCAL_DEPLOY ${WARN} cli_classify_ua.c  \
  -I../ -I../../hhash/  \
  -I../../statsd-c-client-master/ \
  -I../../curl-7.51.0/include/ \
  ../spooky_hash.c  \
  ../load_lkp.c  \
  ../url.c  \
  ../load_classify_ua_map.c  \
  ../classify_ua.c  \
  ../auxil.c \
  ../mmap.c \
  -o ut
mapfile=/opt/ab/ua_to_dev.bin
while read ua 
do 
  echo "Testing $ua"
  ./ut $mapfile "$ua"
  echo ""
done < sample_user_agent.csv
echo "Successfully completed $0 in $PWD"
