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
# Uncomment below to capture server error 
# CFLAGS= $(LFLAGS) -DCAPTURE_SERVER_ERROR 
CFLAGS= $(LFLAGS) 


CC= clang
CC= gcc

CSRCS := $(wildcard *.c)

fil3   := ab_httpd.c sel_ab_httpd.c ab_utils.c 
CSRCS3 := $(filter-out $(fil3), $(CSRCS))
COBJS3 = $(CSRCS3:.c=.o)

filU   := ab_httpd.c sel_ab_httpd.c libe_ab_httpd.c 
USRCS := $(filter-out $(filU), $(CSRCS))
UOBJS = $(USRCS:.c=.o)

UINCS = ab_constants.h  macros.h 

# PARSE_LOG_OBJS = parse_log.o  auxil.o mmap.o
# PARSE_LOG_INCS = ab_constants.h  macros.h 

all : ab_httpd 

# parse_log : $(PARSE_LOG_OBJS) $(PARSE_LOG_INCS) 
# 	$(CC) $(LFLAGS) $(PARSE_LOG_OBJS) -o parse_log 
	
../curl-7.51.0/include/curl/curlbuild.h :
	cd ../curl-7.51.0/ && ./configure && make && cd - 

ab_types.h : ../curl-7.51.0/include/curl/curlbuild.h

auxil.o : ab_types.h
	$(CC) auxil.c -c -o auxil.o $(CFLAGS)  $(INCS)

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

ab_httpd : $(COBJS3) \
	../statsd-c-client-master/libstatsdclient.so \
	../curl-7.51.0/lib/.libs/libcurl.so \
	../lua-5.1.3/src/liblua.a \
	../libevent-2.0.22-stable/.libs/libevent.so 
	$(CC) $(CFLAGS) $(COBJS3) -o ab_httpd  \
	../statsd-c-client-master/libstatsdclient.so \
	../curl-7.51.0/lib/.libs/libcurl.so \
	../lua-5.1.3/src/liblua.a \
	../libevent-2.0.22-stable/.libs/libevent.so \
	-lmysqlclient -lm  -lpthread -ldl


clean:
	rm -r -f ab_httpd $(COBJS1) $(COBJS2) $(COBJS3) 
	make -i -C ../lua-5.1.3/src/ clean 
	make -i -C ../statsd-c-client-master/ clean
	make -i -C ../libevent-2.0.22-stable/ distclean 
	make -i -C ../curl-7.51.0/ distclean 
	echo "ALL CLEAN"

qclean :
	rm -r -f ab_httpd $(COBJS1) $(COBJS2) $(COBJS3) 
