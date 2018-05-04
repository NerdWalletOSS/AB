# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
#
WARN= -g -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic 
# Uncomment following to use openmp. Also, you need
# export LD_LIBRARY_PATH="/usr/local/lib64:$LD_LIBRARY_PATH"
# else you will get 
# else you will get 
# ./test_dt: /usr/lib/x86_64-linux-gnu/libgomp.so.1: version `GOMP_4.0' not
# found (required by ./test_dt)
INCS= -I. -I../../src/ -I../../curl-7.51.0/include/


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

all : utm_kv.so utm_kv strip_url.so

url.o :
	$(CC) -c -o $@ ../../src/url.c $(CFLAGS)  $(INCS)

mmap.o :
	$(CC) -c -o $@ ../../src/mmap.c $(CFLAGS)  $(INCS)

XOBJS = mmap.o url.o

.c.o : 
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

strip_url.so : rfr_aux.o strip_url.o
	$(CC) -shared $(CFLAGS) rfr_aux.o strip_url.o -o strip_url.so \
	-lm  -ldl  

utm_kv.so : utm_kv.o url.o
	$(CC) -shared $(CFLAGS) url.o utm_kv.o  -o utm_kv.so \
	-lm  -ldl  

utm_kv : main.o utm_kv.o url.o
	$(CC) $(CFLAGS) utm_kv.o main.o url.o -o utm_kv \
	-lm  -ldl  

clean :
	rm -r -f utm_kv utm_kv.so *.o _* *.so
