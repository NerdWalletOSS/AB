# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
#
WARN=  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic 
# Uncomment following to use openmp. Also, you need
# export LD_LIBRARY_PATH="/usr/local/lib64:$LD_LIBRARY_PATH"
# else you will get 
# else you will get 
# ./test_ua: /usr/lib/x86_64-linux-gnu/libgomp.so.1: version `GOMP_4.0' not
# found (required by ./test_ua)
INCS= -I. \
	-I../../src/  \
	-I../../curl-7.51.0/include/ \
	-I../../curl-7.51.0/include/curl/ \


DBGFLAGS= -g -DDEBUG
DBGFLAGS= 

OPTFLAGS= -O4 -fopenmp
OPTFLAGS= 
OPTFLAGS= -O4

LFLAGS= $(DBGFLAGS) $(OPTFLAGS)  $(WARN) -DLOCAL_DEPLOY -DMINI
CFLAGS= $(LFLAGS)  -DRASPBERRY_PI

CC= clang
CC= gcc

CSRCS := $(wildcard *.c)
COBJS = $(CSRCS:.c=.o)

filter := main.c mk_ua_to_dev_map.c use_ua_to_dev_map.c
SO_SRCS := $(filter-out $(filter), $(CSRCS))
SO_OBJS = $(SO_SRCS:.c=.o)

all : test_ua ua.so mk_ua_to_dev_map use_ua_to_dev_map

mmap.o :
	$(CC) -c -o $@ ../../src/mmap.c $(CFLAGS)  $(INCS)

spooky_hash.o :
	$(CC) -c -o $@ ../../src/spooky_hash.c $(CFLAGS)  $(INCS)

auxil.o :
	$(CC) -c -o $@ ../../src/auxil.c $(CFLAGS)  $(INCS)

XOBJS = mmap.o auxil.o spooky_hash.o

.c.o : 
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

test_ua : $(SO_OBJS) $(XOBJS)  main.o
	$(CC) $(CFLAGS) $(SO_OBJS) $(XOBJS) main.o $(INCS) -o test_ua  -lm

mk_ua_to_dev_map : $(SO_OBJS) $(XOBJS) mk_ua_to_dev_map.o
	$(CC) $(CFLAGS) $(SO_OBJS) $(XOBJS) mk_ua_to_dev_map.o $(INCS) \
	-o mk_ua_to_dev_map

use_ua_to_dev_map : $(SO_OBJS) $(XOBJS) use_ua_to_dev_map.o
	$(CC) $(CFLAGS) $(SO_OBJS) $(XOBJS) use_ua_to_dev_map.o $(INCS) \
	-o use_ua_to_dev_map


#  -lpthread -ldl  -lgomp


ua.so : $(SO_OBJS) 
	$(CC) -shared $(CFLAGS) $(SO_OBJS) $(INCS) -o ua.so \
	-lm  -lpthread -ldl  -lgomp

clean:
	rm -r -f test_ua $(COBJS)  *.so
	echo "ALL CLEAN"

qclean :
	rm -r -f ua.so test_ua $(COBJS) *.o _*
