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


DBGFLAGS= -g -DDEBUG
DBGFLAGS= 

OPTFLAGS= 
OPTFLAGS= -O4

LFLAGS= $(DBGFLAGS) $(OPTFLAGS)  $(WARN) -DLOCAL_DEPLOY
# Uncomment below to capture server error 
# CFLAGS= $(LFLAGS) -DCAPTURE_SERVER_ERROR 
CFLAGS= $(LFLAGS) 


CC= clang
CC= gcc

LOGGER_OBJS = logger.o  ../src/auxil.o ../src/mmap.o 
LOGGER_INCS = ../src/ab_incs.h

all : ab_logger 

ab_logger : $(LOGGER_OBJS) $(LOGGER_INCS) \
	../jansson-2.10/src/.libs/libjansson.a \
	../libevent-2.0.22-stable/.libs/libevent.so 
	$(CC) $(LFLAGS) $(LOGGER_OBJS) -o ab_logger \
	../jansson-2.10/src/.libs/libjansson.a \
	../libevent-2.0.22-stable/.libs/libevent.so 

../jansson-2.10/src/.libs/libjansson.a :
	cd .. && tar -zxvf jansson-2.10.tar.gz && cd jansson-2.10/ && ./configure && make && cd ../src/

../curl-7.51.0/include/curl/curlbuild.h :
	cd ../curl-7.51.0/ && ./configure && make && cd - 

ab_types.h : ../curl-7.51.0/include/curl/curlbuild.h

ab_incs.h : ../src/ab_types.h ../src/macros.h ../src/ab_constants.h

auxil.o : ab_incs.h
	$(CC) ../src/auxil.c -c -o auxil.o $(CFLAGS)  $(INCS)

mmap.o : ab_incs.h
	$(CC) ../src/mmap.c -c -o mmap.o $(CFLAGS)  $(INCS)

.c.o : ../curl-7.51.0/lib/.libs/libcurl.so 
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

../lua-5.1.3/src/liblua.a :
	make -C ../lua-5.1.3/src/ linux

../curl-7.51.0/lib/.libs/libcurl.so : 
	cd ../curl-7.51.0/ && ./configure && make && cd - 

../libevent-2.0.22-stable/.libs/libevent.so : 
	cd ../libevent-2.0.22-stable/ &&  ./configure && make && cd - 

$(LOGGER_OBJS) : ../libevent-2.0.22-stable/.libs/libevent.so

$(COBJS) : ../libevent-2.0.22-stable/.libs/libevent.so

clean:
	rm -r -f ab_logger \
	$(COBJS1) $(COBJS2) $(COBJS3) $(LOGGER_OBJS) 
	make -i -C ../libevent-2.0.22-stable/ distclean 
	make -i -C ../jansson-2.10/ distclean 
	make -i -C ../curl-7.51.0/ distclean 
	echo "ALL CLEAN"

qclean :
	rm -r -f ab_logger \
	$(COBJS1) $(COBJS2) $(COBJS3) $(LOGGER_OBJS) 
