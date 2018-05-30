# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
#
WARN=  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic 
INCS= -I. \
	-I../src/ \
	-I../libevent-2.0.22-stable/include/event2/ \
	-I../libevent-2.0.22-stable/include/ \
	-I../jansson-2.10/src/ \
	-I../statsd-c-client-master/ \
	-I../curl-7.51.0/include/ \
	-I../curl-7.51.0/include/curl/ \
	-I../dt_interpreter/src/  \
	-I../libmaxminddb-1.3.2/include/ \
	-I../registered-domain-libs/C/ \
	-I../libyuarel/ \
	-I../libmaxminddb-1.3.2/projects/VS12/


DBGFLAGS= 
DBGFLAGS= -g -DDEBUG

OPTFLAGS= -O4
OPTFLAGS= 

LFLAGS= $(DBGFLAGS) $(OPTFLAGS)  $(WARN) -DLOCAL_DEPLOY
# Uncomment below to capture server error 
# CFLAGS= $(LFLAGS) -DCAPTURE_SERVER_ERROR 
CFLAGS= $(LFLAGS) 


CC= clang
CC= gcc

PBLASTER_OBJS = \
	 restart.o execute.o setup_curl.o pblaster.o  add_tests.o \
	../src/auxil.o \
	../src/mmap.o  \
	../src/make_guid.o  \
	../src/spooky_hash.o  \

SBLASTER_OBJS = \
	restart.o setup_curl.o sblaster.o  execute.o add_tests.o \
	../src/auxil.o \
	../src/mmap.o  \
	../src/make_guid.o  \
	../src/spooky_hash.o  \

ADD_TEST_OBJS = \
	ut_add_tests.o setup_curl.o add_tests.o  execute.o \
	../src/auxil.o \
	../src/mmap.o  \
	../src/make_guid.o  \
	../src/spooky_hash.o  \


BLASTER_INCS = ../src/ab_incs.h

all : sblaster pblaster add_tests

add_tests : $(ADD_TEST_OBJS) $(BLASTER_INCS) 
	$(CC) $(LFLAGS) $(ADD_TEST_OBJS) -o add_tests  \
	../src/libs/libcurl.so 

pblaster : $(PBLASTER_OBJS) $(BLASTER_INCS) 
	$(CC) $(LFLAGS) $(PBLASTER_OBJS) -o pblaster \
	../src/libs/libcurl.so -lpthread

sblaster : $(SBLASTER_OBJS) $(BLASTER_INCS) 
	$(CC) $(LFLAGS) $(SBLASTER_OBJS) -o sblaster \
	../src/libs/libcurl.so 


../jansson-2.10/src/.libs/libjansson.a :
	cd .. && tar -zxvf jansson-2.10.tar.gz && cd jansson-2.10/ && ./configure && make && cd ../src/

auxil.o : ab_incs.h
	$(CC) ../src/auxil.c -c -o auxil.o $(CFLAGS)  $(INCS)

mmap.o : ab_incs.h
	$(CC) ../src/mmap.c -c -o mmap.o $(CFLAGS)  $(INCS)

make_guid.o : 
	$(CC) ../src/make_guid.c -c -o make_guid.o $(CFLAGS)  $(INCS)

spooky_hash.o : 
	$(CC) ../src/spooky_hash.c -c -o spooky_hash.o $(CFLAGS)  $(INCS)

.c.o : 
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

clean:
	rm -r -f sblaster pblaster *.o

