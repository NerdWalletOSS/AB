# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
#
WARN=  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic 
INCS= -I. \
	-I../libevent-2.0.22-stable/include/event2/ \
	-I../libevent-2.0.22-stable/include/ \
	-I../statsd-c-client-master/ \
	-I../lua-5.1.3/src/ \
	-I../curl-7.51.0/include/ \
	-I../curl-7.51.0/include/curl/ 


DBGFLAGS= 
DBGFLAGS= -g -DDEBUG

OPTFLAGS= -O4
OPTFLAGS= 

LFLAGS= $(DBGFLAGS) $(OPTFLAGS)  $(WARN) -DLOCAL_DEPLOY
CFLAGS= $(LFLAGS) -DCAPTURE_SERVER_ERROR 

CC= clang
CC= gcc

CSRCS := $(wildcard *.c)
COBJS = $(CSRCS:.c=.o)

all : ab_httpd 

../curl-7.51.0/include/curl/curlbuild.h :
	cd ../curl-7.51.0/ && ./configure && make && cd - 

ab_incs.h : ../curl-7.51.0/include/curl/curlbuild.h

.c.o : ../curl-7.51.0/lib/.libs/libcurl.so  
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

../lua-5.1.3/src/liblua.a :
	make -C ../lua-5.1.3/src/ linux

../curl-7.51.0/lib/.libs/libcurl.so : 
	cd ../curl-7.51.0/ && ./configure && make && cd - 

../libevent-2.0.22-stable/.libs/libevent.so : 
	cd ../libevent-2.0.22-stable/ &&  ./configure && make && cd - 

$(COBJS) : ../libevent-2.0.22-stable/.libs/libevent.so

../statsd-c-client-master/libstatsdclient.so :
	make -C ../statsd-c-client-master/

ab_httpd : $(COBJS) \
	../statsd-c-client-master/libstatsdclient.so \
	../curl-7.51.0/lib/.libs/libcurl.so \
	../lua-5.1.3/src/liblua.a \
	../libevent-2.0.22-stable/.libs/libevent.so 
	$(CC) $(CFLAGS) $(COBJS) -o ab_httpd  \
	../statsd-c-client-master/libstatsdclient.so \
	../curl-7.51.0/lib/.libs/libcurl.so \
	../lua-5.1.3/src/liblua.a \
	../libevent-2.0.22-stable/.libs/libevent.so \
	-lm  -lpthread -ldl 

clean:
	rm -r -f ab_httpd $(COBJS) 
	make -i -C ../lua-5.1.3/src/ clean 
	make -i -C ../statsd-c-client-master/ clean
	make -i -C ../libevent-2.0.22-stable/ distclean 
	make -i -C ../curl-7.51.0/ distclean 
	echo "ALL CLEAN"

qclean :
	rm -r -f ab_httpd $(COBJS) *.o _*
