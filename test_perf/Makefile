# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
#
WARN=  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic -fopenmp
INCS= -I. \
	-I../src/ \
	-I../curl-7.51.0/include/ \
	-I../curl-7.51.0/include/curl/ \

DBGFLAGS= -g 
DBGFLAGS= 

OPTFLAGS= 
OPTFLAGS= -O4

LFLAGS= $(DBGFLAGS) $(OPTFLAGS)  $(WARN) 
# Uncomment below to capture server error 
# CFLAGS= $(LFLAGS) -DCAPTURE_SERVER_ERROR 
CFLAGS= $(LFLAGS) 


CC= clang
CC= gcc

PBLASTER_OBJS = \
	 restart.o execute.o setup_curl.o pblaster.o  add_tests.o \
	 read_test_info.o hammer.o \
	../src/auxil.o \
	../src/mmap.o  \
	../src/make_guid.o  \
	../src/spooky_hash.o  \

SBLASTER_OBJS = \
	restart.o setup_curl.o sblaster.o  execute.o add_tests.o \
	read_test_info.o hammer.o \
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

all : sblaster pblaster add_tests sdt pdt

add_tests : $(ADD_TEST_OBJS) $(BLASTER_INCS) 
	$(CC) $(LFLAGS) $(ADD_TEST_OBJS) -o add_tests  \
	../src/libs/libcurl.so 

pblaster : $(PBLASTER_OBJS) $(BLASTER_INCS) 
	$(CC) $(LFLAGS) $(PBLASTER_OBJS) -o pblaster \
	../src/libs/libcurl.so -lpthread

sblaster : $(SBLASTER_OBJS) $(BLASTER_INCS) 
	$(CC) $(LFLAGS) $(SBLASTER_OBJS) -o sblaster \
	../src/libs/libcurl.so 

pdt : pdt.o mmap.o auxil.o setup_curl.o execute.o
	$(CC) $(LFLAGS) mmap.o pdt.o execute.o setup_curl.o auxil.o -o pdt \
	../src/libs/libcurl.so 

sdt : sdt.o mmap.o auxil.o
	$(CC) $(LFLAGS) mmap.o sdt.o auxil.o -o sdt \
	../src/libs/libcurl.so 

auxil.o : 
	$(CC) ../src/auxil.c -c -o auxil.o $(CFLAGS)  $(INCS)

mmap.o : 
	$(CC) ../src/mmap.c -c -o mmap.o $(CFLAGS)  $(INCS)

make_guid.o : 
	$(CC) ../src/make_guid.c -c -o make_guid.o $(CFLAGS)  $(INCS)

spooky_hash.o : 
	$(CC) ../src/spooky_hash.c -c -o spooky_hash.o $(CFLAGS)  $(INCS)

.c.o : 
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

clean:
	rm -r -f sblaster pblaster *.o

