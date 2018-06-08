# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
#
WARN=  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic 
# Uncomment following to use openmp. Also, you need
# export LD_LIBRARY_PATH="/usr/local/lib64:$LD_LIBRARY_PATH"
# else you will get 
# else you will get 
# ./test_dt: /usr/lib/x86_64-linux-gnu/libgomp.so.1: version `GOMP_4.0' not
# found (required by ./test_dt)
INCS= -I.  -I../../src/ 

DBGFLAGS= -g -DDEBUG
DBGFLAGS= 

OPTFLAGS= -O4
OPTFLAGS= 
OPTFLAGS= -O4 -fopenmp

LFLAGS= $(DBGFLAGS) $(OPTFLAGS)  $(WARN) -DLOCAL_DEPLOY 
# CFLAGS= $(LFLAGS)  -DRASPBERRY_PI
CFLAGS= $(LFLAGS) 

CC= clang
CC= gcc

CSRCS := $(wildcard *.c)
COBJS = $(CSRCS:.c=.o)

SO_SRCS := $(filter-out main.c, $(CSRCS))
SO_OBJS = $(SO_SRCS:.c=.o)

all : test_dt dt.so

mmap.o :
	$(CC) -c -o $@ ../../src/mmap.c $(CFLAGS)  $(INCS)

auxil.o :
	$(CC) -c -o $@ ../../src/auxil.c $(CFLAGS)  $(INCS)

XOBJS = mmap.o auxil.o

.c.o : 
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

test_dt : $(COBJS) $(XOBJS) 
	$(CC) $(CFLAGS) $(COBJS) $(XOBJS) $(INCS) -o test_dt  -lm \
	-lpthread -ldl  -lgomp


dt.so : $(SO_OBJS) 
	$(CC) -shared $(CFLAGS) $(SO_OBJS) $(INCS) -o dt.so \
	-lm  -lpthread -ldl  -lgomp

clean:
	rm -r -f test_dt $(COBJS)  *.so
	echo "ALL CLEAN"

qclean :
	rm -r -f dt.so test_dt $(COBJS) *.o _*
