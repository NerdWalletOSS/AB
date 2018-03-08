# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
#
WARN=  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic 
# Uncomment following to use openmp. Also, you need
# export LD_LIBRARY_PATH="/usr/local/lib64:$LD_LIBRARY_PATH"
# else you will get 
# else you will get 
# ./test_dt: /usr/lib/x86_64-linux-gnu/libgomp.so.1: version `GOMP_4.0' not
# found (required by ./test_dt)
INCS= -I. -I../../src/


DBGFLAGS= -g -DDEBUG
DBGFLAGS= 

OPTFLAGS= 
OPTFLAGS= -O4 -fopenmp
OPTFLAGS= -O4

LFLAGS= $(DBGFLAGS) $(OPTFLAGS)  $(WARN) -DLOCAL_DEPLOY
CFLAGS= $(LFLAGS) 

CC= clang
CC= gcc

CSRCS := $(wildcard *.c)
COBJS = $(CSRCS:.c=.o)

all : test_dt

mmap.o :
	$(CC) -c -o $@ ../../src/mmap.c $(CFLAGS)  $(INCS)

XOBJS = mmap.o

.c.o : 
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

test_dt : $(COBJS) $(XOBJS) 
	$(CC) $(CFLAGS) $(COBJS) $(XOBJS) -o test_dt  \
	-lm  -lpthread -ldl  -lgomp

clean:
	rm -r -f test_dt $(COBJS) 
	echo "ALL CLEAN"

qclean :
	rm -r -f ab_httpd $(COBJS) *.o _*
