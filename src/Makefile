#
# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
WARN=  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic -Wno-aggregate-return -Wno-unused-parameter
INCS= -I. \
	-I../libevent-2.0.22-stable/include/event2/ \
	-I../libevent-2.0.22-stable/include/ \
	-I../statsd-c-client-master/ \
	-I../LuaJIT-2.1.0-beta3/src/ \
	-I../curl-7.51.0/include/ \
	-I../curl-7.51.0/include/curl/ \
	-I../dt_interpreter/src/  \
	-I../utm_kv/src/  \
	-I../libmaxminddb-1.3.2/include/ \
	-I../registered-domain-libs/C/ \
	-I../libyuarel/ \
	-I../libmaxminddb-1.3.2/projects/VS12/ \
	-I../librdkafka/src/ \
	-I../jansson-2.10/src/ \
	-I ../mysql-connector-c-6.1.6-src/include/ \
	-I../USER_AGENT_CLASSIFIER/src/ 

DBGFLAGS= 
DBGFLAGS= -g -DDEBUG

OPTFLAGS= -O4
OPTFLAGS= 

LFLAGS= $(DBGFLAGS) $(OPTFLAGS)  $(WARN) \
	-DKAFKA \
	-DMAXMIND \
	-DUTM_KV \
	#
#	-DAB_AS_KAFKA \
#	-DRASPBERRY_PI

#	-DGET_HOST_DOMAIN \

# set one of following 
CFLAGS= $(LFLAGS) -DCAPTURE_SERVER_ERROR 
CFLAGS= $(LFLAGS) 

CC= clang
CC= gcc

CSRCS := $(wildcard *.c)
COBJS = $(CSRCS:.c=.o)

all : ab_httpd \
	libspooky_hash.so \
	../lua/ab_consts.lua \
	../lua/ab_ffi.lua \
	../dt_interpreter/src/dt.so  \
	../USER_AGENT_CLASSIFIER/src/ua.so 

./libs:
	mkdir libs

../LuaJIT-2.1.0-beta3/src/libluajit.a:
		cd ../ && \
		rm -rf LuaJIT-2.1.0-beta3 && \
	 	tar -xvzf LuaJIT-2.1.0-beta3.tar.gz && \
	 	cd LuaJIT-2.1.0-beta3/ && \
		sed -i '114s/#//' src/Makefile && \
		make TARGET_FLAGS=-pthread

../curl-7.51.0/include/curl/curlbuild.h :
	cd ../curl-7.51.0/ && ./configure && make && cd - 

../jansson-2.10/src/.libs/libjansson.so :
	cd .. && tar -zxvf jansson-2.10.tar.gz && cd jansson-2.10/ && ./configure && make && cd ../src/

ab_auxil.o : ../curl-7.51.0/include/curl/curlbuild.h ../librdkafka/src/librdkafka.so ../mysql-connector-c-6.1.6-src/libmysql/libmysqlclient.so

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


.c.o : ../curl-7.51.0/lib/.libs/libcurl.so  ../mysql-connector-c-6.1.6-src/libmysql/libmysqlclient.so
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS) $(DBGFLAGS)

../lua-5.1.3/src/liblua.a :
	make -C ../lua-5.1.3/src/ linux

../dt_interpreter/src/dt.so : ./libs/libmaxminddb.so ../jansson-2.10/src/.libs/libjansson.so
	make -C ../dt_interpreter/src/

../USER_AGENT_CLASSIFIER/src/ua.so : 
	make -C ../USER_AGENT_CLASSIFIER/src/

../utm_kv/src/utm_kv.so : ./libs/libmaxminddb.so
	make -C ../utm_kv/src/

../curl-7.51.0/lib/.libs/libcurl.so : 
	cd ../curl-7.51.0/ && ./configure && make && cd - 

./libs/libcurl.so: ./libs ../curl-7.51.0/lib/.libs/libcurl.so ./libs
	cp ../curl-7.51.0/lib/.libs/libcurl.so ./libs

../libevent-2.0.22-stable/.libs/libevent.so : 
	cd ../libevent-2.0.22-stable/ &&  ./configure && make && cd - 

./libs/libevent.so: ./libs ../libevent-2.0.22-stable/.libs/libevent.so
	cp ../libevent-2.0.22-stable/.libs/libevent.so ./libs/libevent.so

$(COBJS) : ../LuaJIT-2.1.0-beta3/src/libluajit.a ./libs/libevent.so ./libs/libmaxminddb.so ./libs/librdkafka.so ./libs/libcurl.so ../jansson-2.10/src/.libs/libjansson.so ../mysql-connector-c-6.1.6-src/libmysql/libmysqlclient.so



../statsd-c-client-master/libstatsdclient.so :
	make -C ../statsd-c-client-master/

./libs/libstatsdclient.so: ./libs ../statsd-c-client-master/libstatsdclient.so
	cp ../statsd-c-client-master/libstatsdclient.so ./libs/libstatsdclient.so

../mysql-connector-c-6.1.6-src/libmysql/libmysqlclient.so:
	cd ../ && \
		rm -rf mysql-connector-c-6.1.6-src && \
		tar -xvzf mysql-connector-c-6.1.6-src.tar.gz && \
		cd mysql-connector-c-6.1.6-src && cmake -G "Unix Makefiles" && \
		make

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

./libs/dt.so: ./libs ../dt_interpreter/src/dt.so
	cp ../dt_interpreter/src/dt.so ./libs/dt.so

./libs/ua.so: ./libs ../USER_AGENT_CLASSIFIER/src/ua.so
	cp ../USER_AGENT_CLASSIFIER/src/ua.so ./libs/ua.so

./libs/utm_kv.so: ./libs ../utm_kv/src/utm_kv.so
	cp ../utm_kv/src/utm_kv.so ./libs/utm_kv.so

./libs/librdkafka.so: ./libs ../librdkafka/src/librdkafka.so
	cp ../librdkafka/src/librdkafka.so ./libs/librdkafka.so

#	../registered-domain-libs/C/regdom.o 

ab_httpd : $(COBJS)  \
	../lua/ab_consts.lua \
	../lua/ab_ffi.lua \
	./libs/libstatsdclient.so \
	./libs/libcurl.so \
	../LuaJIT-2.1.0-beta3/src/libluajit.a \
	../jansson-2.10/src/.libs/libjansson.so \
	./libs/dt.so \
	./libs/ua.so \
	./libs/utm_kv.so \
	./libs/libevent.so \
	../libyuarel/yuarel.o \
	./libs/libmysqlclient.so \
	./libs/libmaxminddb.so \
	./libs/librdkafka.so \
	./libs/libmysqlclient.so 
	$(CC) $(CFLAGS) $(COBJS) \
	  -o ab_httpd  \
	./libs/libstatsdclient.so \
	./libs/libcurl.so \
	../libyuarel/yuarel.o \
	../LuaJIT-2.1.0-beta3/src/libluajit.a \
	../jansson-2.10/src/.libs/libjansson.so \
	./libs/dt.so \
	./libs/ua.so \
	./libs/utm_kv.so \
	./libs/libevent.so \
	./libs/libmaxminddb.so \
	./libs/librdkafka.so \
	./libs/libmysqlclient.so \
	-lm  -lpthread -ldl 

../librdkafka/src/librdkafka.so:
	cd ../ && rm -rf librdkafka &&  tar -xvzf librdkafka.tar.gz && cd librdkafka && ./configure && make

libspooky_hash.so:
	gcc spooky_hash.c get_test_idx.c add_fake_test.c convert_str_to_u64.c $(CFLAGS) $(INCS) -shared -fPIC -o libspooky_hash.so

../lua/ab_consts.lua:
	bash gen_consts.sh

../lua/ab_ffi.lua:
	bash gen_ab_ffi.sh


clean:
	rm -r -f ab_httpd $(COBJS) libspooky_hash.so ab_consts.lua ab_ffi.lua
	make -i -C ../LuaJIT-2.1.0-beta3/src/ clean 
	make -i -C ../statsd-c-client-master/ clean
	make -i -C ../libevent-2.0.22-stable/ distclean 
	make -i -C ../curl-7.51.0/ distclean 
	make -C ../dt_intepreter/src clean
	make -C ../USER_AGENT_CLASSIFIER/src clean
	rm -f ../dt_interpreter/src/*.so 
	rm -f ../USER_AGENT_CLASSIFIER/src/*.so 
	rm -r -f ../librdkafka/* # TODO P3 Check with Indrajeet
	rm -r -f ../jansson-2.10/* # TODO P3 Check with Indrajeet
	rm -r -f ../libmaxminddb-1.3.2/* # TODO P3 Check with Indrajeet
	rm -f ../lua/ab_ffi.lua ../lua/ab_consts.lua
	echo "ALL CLEAN"

qclean :
	rm -r -f ab_httpd $(COBJS) *.o _*
	rm -f ../lua/ab_ffi.lua ../lua/ab_consts.lua

