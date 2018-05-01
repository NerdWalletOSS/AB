# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
#
WARN=  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic -Wno-aggregate-return
INCS= -I. \
	-I../libevent-2.0.22-stable/include/event2/ \
	-I../libevent-2.0.22-stable/include/ \
	-I../statsd-c-client-master/ \
	-I../luajit-2.0.5/src/ \
	-I../curl-7.51.0/include/ \
	-I../curl-7.51.0/include/curl/ \
	-I../dt_interpreter/src/  \
	-I../utm_kv/src/  \
	-I../libmaxminddb-1.3.2/include/ \
	-I../registered-domain-libs/C/ \
	-I../libyuarel/ \
	-I../libmaxminddb-1.3.2/projects/VS12/ \
	-I../librdkafka/src

DBGFLAGS= 
DBGFLAGS= -g -DDEBUG

OPTFLAGS= -O4
OPTFLAGS= 

LFLAGS= $(DBGFLAGS) $(OPTFLAGS)  $(WARN) -DNW_SPECIFIC

# set one of following 
CFLAGS= $(LFLAGS) -DCAPTURE_SERVER_ERROR 
CFLAGS= $(LFLAGS) 

CC= clang
CC= gcc

CSRCS := $(wildcard *.c)
COBJS = $(CSRCS:.c=.o)

all : ab_httpd libspooky_hash.so ab_consts.lua ab_ffi.lua ../dt_interpreter/src/dt.so

./libs:
	mkdir libs

../luajit-2.0.5/src/libluajit.a :
	make -C ../luajit-2.0.5

../curl-7.51.0/include/curl/curlbuild.h :
	cd ../curl-7.51.0/ && ./configure && make && cd - 

ab_incs.h : ../curl-7.51.0/include/curl/curlbuild.h

update_config.c : load_rf.h load_mdl.h load_rf.c load_mdl.c 

load_rf.h : load_dt.h
	cat load_dt.h | \
	sed s'/dt_types/XXXX/'g | \
	sed s'/dt/rf/'g | \
	sed s'/DT_REC/RF_REC/'g | \
	sed s'/XXXX/dt_types/'g > load_rf.h

load_rf.c : load_dt.c
	cat load_dt.c | sed s'/DT_REC/RF_REC/'g | \
	sed s'/#include "load_dt.h"/#include "load_rf.h"/'g | \
	sed s'/dt/rf/'g > load_rf.c

load_rf.o : load_rf.c load_rf.h
	$(CC) -c load_rf.c -o load_rf.o $(CFLAGS) $(INCS)

load_mdl.h : load_dt.h
	cat load_dt.h | \
	sed s'/dt_types/XXXX/'g | \
	sed s'/dt/mdl/'g | \
	sed s'/DT_REC/MDL_REC/'g | \
	sed s'/XXXX/dt_types/'g > load_mdl.h

load_mdl.c : load_dt.c
	cat load_dt.c | sed s'/DT_REC/MDL_REC/'g | \
	sed s'/#include "load_dt.h"/#include "load_mdl.h"/'g | \
	sed s'/dt/mdl/'g > load_mdl.c

load_mdl.o : load_mdl.c load_mdl.h
	$(CC) -c load_mdl.c -o load_mdl.o $(CFLAGS) $(INCS)


.c.o : ../curl-7.51.0/lib/.libs/libcurl.so  
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

../lua-5.1.3/src/liblua.a :
	make -C ../lua-5.1.3/src/ linux

../dt_interpreter/src/dt.so : ../libmaxminddb-1.3.2/src/.libs/libmaxminddb.so
	make -C ../dt_interpreter/src/

../utm_kv/src/utm_kv.so : ../libmaxminddb-1.3.2/src/.libs/libmaxminddb.so
	make -C ../utm_kv/src/

../curl-7.51.0/lib/.libs/libcurl.so : 
	cd ../curl-7.51.0/ && ./configure && make && cd - 

./libs/libcurl.so: ./libs ../curl-7.51.0/lib/.libs/libcurl.so ./libs
	cp ../curl-7.51.0/lib/.libs/libcurl.so ./libs

../libevent-2.0.22-stable/.libs/libevent.so : 
	cd ../libevent-2.0.22-stable/ &&  ./configure && make && cd - 

./libs/libevent.so: ./libs ../libevent-2.0.22-stable/.libs/libevent.so
	cp ../libevent-2.0.22-stable/.libs/libevent.so ./libs/libevent.so

$(COBJS) : ../libevent-2.0.22-stable/.libs/libevent.so ../libmaxminddb-1.3.2/src/.libs/libmaxminddb.so

../statsd-c-client-master/libstatsdclient.so :
	make -C ../statsd-c-client-master/

./libs/libstatsdclient.so: ./libs ../statsd-c-client-master/libstatsdclient.so
	cp ../statsd-c-client-master/libstatsdclient.so ./libs/libstatsdclient.so

../mysql-connector-c-6.1.6-src/libmysql/libmysqlclient.so:
	cd ../ && rm -rf mysql-connector-c-6.1.6-src && tar -xvzf mysql-connector-c-6.1.6-src.tar.gz && cd mysql-connector-c-6.1.6-src && cmake -G "Unix Makefiles"
	make -C ../mysql-connector-c-6.1.6-src
		 -
./libs/libmysqlclient.so: ./libs ../mysql-connector-c-6.1.6-src/libmysql/libmysqlclient.so
	cp ../mysql-connector-c-6.1.6-src/libmysql/libmysqlclient.so ./libs/libmysqlclient.so

../libyuarel/yuarel.o : ../libyuarel/yuarel.c ../libyuarel/yuarel.h
	make -C ../libyuarel/

../registered-domain-libs/C/regdom.o : 
	make -C ../registered-domain-libs/C/

../libmaxminddb-1.3.2/src/.libs/libmaxminddb.so:
	cd ../ && rm -rf libmaxminddb-1.3.2 && tar -xvzf libmaxminddb-1.3.2.tar.gz && cd libmaxminddb-1.3.2/ && ./configure && make	

./libs/libmaxminddb.so: ./libs ../libmaxminddb-1.3.2/src/.libs/libmaxminddb.so
	cp ../libmaxminddb-1.3.2/src/.libs/libmaxminddb.so ./libs/libmaxminddb.so

ab_httpd : $(COBJS)  \
	./libs/libstatsdclient.so \
	./libs/libcurl.so \
	../luajit-2.0.5/src/libluajit.a \
	../dt_interpreter/src/dt.so \
	../utm_kv/src/utm_kv.so \
	./libs/libevent.so \
	../libyuarel/yuarel.o \
	../registered-domain-libs/C/regdom.o \
	./libs/libmysqlclient.so \
	./libs/libmaxminddb.so \
  ../librdkafka/src/librdkafka.so
	$(CC) $(CFLAGS) $(COBJS) \
	  -o ab_httpd  \
	./libs/libstatsdclient.so \
	./libs/libcurl.so \
	../libyuarel/yuarel.o \
	../registered-domain-libs/C/regdom.o \
	../luajit-2.0.5/src/libluajit.a \
	../dt_interpreter/src/dt.so \
	../utm_kv/src/utm_kv.so \
	./libs/libevent.so \
	./libs/libmaxminddb.so \
	../librdkafka/src/librdkafka.so \
	-lm  -lpthread -ldl   

libspooky_hash.so:
	gcc spooky_hash.c get_test_idx.c convert_str_to_u64.c $(CFLAGS) $(INCS) -shared -fPIC -o libspooky_hash.so

ab_consts.lua:
	bash gen_consts.sh

ab_ffi.lua:
	bash gen_ab_ffi.sh


clean:
	rm -r -f ab_httpd $(COBJS) libspooky_hash.so ab_consts.lua ab_ffi.lua
	make -i -C ../luajit-2.0.5/src/ clean 
	make -i -C ../statsd-c-client-master/ clean
	make -i -C ../libevent-2.0.22-stable/ distclean 
	make -i -C ../curl-7.51.0/ distclean 
	echo "ALL CLEAN"

qclean :
	rm -r -f ab_httpd $(COBJS) *.o _*
