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
CFLAGS= $(LFLAGS) 

CC= clang
CC= gcc

CSRCS := $(wildcard *.c)
COBJS = $(CSRCS:.c=.o)

SO_CSRCS := $(filter-out main.c, $(CSRCS))
SO_COBJS = $(SO_CSRCS:.c=.o)

all : test_ua ua.so

mmap.o :
	$(CC) -c -o $@ ../../src/mmap.c $(CFLAGS)  $(INCS)

auxil.o :
	$(CC) -c -o $@ ../../src/auxil.c $(CFLAGS)  $(INCS)

XOBJS = mmap.o auxil.o

.c.o : 
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

test_ua : $(COBJS) $(XOBJS) 
	$(CC) $(CFLAGS) $(COBJS) $(XOBJS) $(INCS) -o test_ua

ua.so : $(SO_COBJS) 
	$(CC) -shared $(CFLAGS) $(SO_COBJS) $(INCS) -o ua.so \
	-lm  -lpthread -ldl  -lgomp

clean :
	rm -r -f ua.so test_ua $(COBJS) *.o _*
