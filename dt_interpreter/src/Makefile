# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
#
WARN=  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic 
# Uncomment following to use openmp. Also, you need
# export LD_LIBRARY_PATH="/usr/local/lib64:$LD_LIBRARY_PATH"
# else you will get 
# else you will get 
# ./test_dt: /usr/lib/x86_64-linux-gnu/libgomp.so.1: version `GOMP_4.0' not
# found (required by ./test_dt)
INCS= -I. \
	-I../../src/  \
	-I../../curl-7.51.0/include/ \
	-I../../curl-7.51.0/include/curl/ \


DBGFLAGS= 
DBGFLAGS= -g -DDEBUG

OPTFLAGS= -O4
OPTFLAGS= -O4 -fopenmp
OPTFLAGS= 

LFLAGS= $(DBGFLAGS) $(OPTFLAGS)  $(WARN) -DLOCAL_DEPLOY -DMINI
CFLAGS= $(LFLAGS) 

CC= clang
CC= gcc

CSRCS := $(wildcard *.c)
COBJS = $(CSRCS:.c=.o)

SO_SRCS := $(filter-out main.c, $(CSRCS))
SO_COBJS = $(SOCSRCS:.c=.o)

all : test_dt dt.so

mmap.o :
	$(CC) -c -o $@ ../../src/mmap.c $(CFLAGS)  $(INCS)

XOBJS = mmap.o

.c.o : 
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

test_dt : $(COBJS) $(XOBJS) 
	$(CC) $(CFLAGS) $(COBJS) $(XOBJS) $(INCS) -o test_dt  \
	-lm  -lpthread -ldl  -lgomp

dt.so : $(SO_COBJS) 
	$(CC) -shared $(CFLAGS) $(SO_COBJS) $(INCS) -o dt.so \
	-lm  -lpthread -ldl  -lgomp

clean:
	rm -r -f test_dt $(COBJS) 
	echo "ALL CLEAN"

qclean :
	rm -r -f dt.so test_dt $(COBJS) *.o _*
