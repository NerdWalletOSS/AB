#!?bin/bash
set -e
WARN="  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic "
gcc -g -DLOCAL_DEPLOY ${WARN} process_user_agents.c \
  -I../ -I../../hhash/  \
  -I../../statsd-c-client-master/ \
  -I../../curl-7.51.0/include/ \
  ../spooky_hash.c  \
  ../load_lkp.c  \
  ../auxil.c \
  ../mmap.c 
opfile=_xx
rm -f $opfile
mapfile=rs_tmp_user_agent000
test -f $mapfile
valgrind --leak-check=full ./a.out $mapfile $opfile
cp $opfile /opt/ab/ua_to_dev.bin
echo "Successfully completed $0 in $PWD"
