# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
#
WARN= -g -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic 
# Uncomment following to use openmp. Also, you need
# export LD_LIBRARY_PATH="/usr/local/lib64:$LD_LIBRARY_PATH"
# else you will get 
# else you will get 
# ./test_dt: /usr/lib/x86_64-linux-gnu/libgomp.so.1: version `GOMP_4.0' not
# found (required by ./test_dt)
INCS= -I. -I../../src/


DBGFLAGS= 
DBGFLAGS= -g -DDEBUG

OPTFLAGS= -O4
OPTFLAGS= 

LFLAGS= $(DBGFLAGS) $(OPTFLAGS)  $(WARN) -DLOCAL_DEPLOY
CFLAGS= $(LFLAGS) 

CC= clang
CC= gcc

CSRCS := $(wildcard *.c)
COBJS = $(CSRCS:.c=.o)

SO_CSRCS := $(filter-out main.c, $(CSRCS))
SO_COBJS = $(SO_CSRCS:.c=.o)

all : utm_kv.so utm_kv

url.o :
	$(CC) -c -o $@ ../../src/url.c $(CFLAGS)  $(INCS)

mmap.o :
	$(CC) -c -o $@ ../../src/mmap.c $(CFLAGS)  $(INCS)

XOBJS = mmap.o url.o

.c.o : 
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

utm_kv.so : $(SO_COBJS) 
	$(CC) -shared $(CFLAGS) $(SO_COBJS) -o utm_kv.so \
	-lm  -lpthread -ldl  

utm_kv.so : $(COBJS)  $(XOBJS)
	$(CC) $(CFLAGS) $(XOBJS) $(COBJS) -o utm_kv \
	-lm  -lpthread -ldl  

clean:
	rm -r -f test_dt $(COBJS) 
	echo "ALL CLEAN"

qclean :
	rm -r -f utm_kv utm_kv.so $(COBJS) *.o _*
