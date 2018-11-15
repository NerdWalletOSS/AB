#
# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
WARN=  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic -Wno-aggregate-return -Wno-unused-parameter
INCS= -I. \
	-I../libevent-2.0.22-stable/include/event2/ \
	-I../libevent-2.0.22-stable/include/ \
	-I../LuaJIT-2.1.0-beta3/src/ \
	-I../dt_interpreter/src/  \
	-I../jansson-2.10/src/ \

DBGFLAGS= 
DBGFLAGS= -g -DDEBUG

OPTFLAGS= -O4
OPTFLAGS= 

LFLAGS= $(DBGFLAGS) $(OPTFLAGS)  $(WARN) 

CFLAGS= $(LFLAGS) 

CC= clang
CC= gcc

CSRCS := $(wildcard *.c)
COBJS = $(CSRCS:.c=.o)

all : dt_httpd \
	../dt_interpreter/src/dt.so  \

./libs:
	mkdir libs

../LuaJIT-2.1.0-beta3/src/libluajit.a:
		cd ../ && \
		rm -rf LuaJIT-2.1.0-beta3 && \
	 	tar -xvzf LuaJIT-2.1.0-beta3.tar.gz && \
	 	cd LuaJIT-2.1.0-beta3/ && \
		sed -i '114s/#//' src/Makefile && \
		make TARGET_FLAGS=-pthread

../jansson-2.10/src/.libs/libjansson.so :
	cd .. && tar -zxvf jansson-2.10.tar.gz && cd jansson-2.10/ && ./configure && make && cd ../src/

dt_auxil.o : 

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


.c.o : 
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS) $(DBGFLAGS)

../lua-5.1.3/src/liblua.a :
	make -C ../lua-5.1.3/src/ linux

../dt_interpreter/src/dt.so : ../jansson-2.10/src/.libs/libjansson.so
	make -C ../dt_interpreter/src/

../libevent-2.0.22-stable/.libs/libevent.so : 
	cd ../libevent-2.0.22-stable/ &&  ./configure && make && cd - 

./libs/libevent.so: ./libs ../libevent-2.0.22-stable/.libs/libevent.so
	cp ../libevent-2.0.22-stable/.libs/libevent.so ./libs/libevent.so

$(COBJS) : ../LuaJIT-2.1.0-beta3/src/libluajit.a \
	./libs/libevent.so \
	../jansson-2.10/src/.libs/libjansson.so


./libs/dt.so: ./libs ../dt_interpreter/src/dt.so
	cp ../dt_interpreter/src/dt.so ./libs/dt.so

dt_httpd : $(COBJS)  \
	../LuaJIT-2.1.0-beta3/src/libluajit.a \
	../jansson-2.10/src/.libs/libjansson.so \
	./libs/dt.so \
	./libs/libevent.so
	$(CC) $(CFLAGS) $(COBJS) \
	  -o dt_httpd  \
	./libs/libevent.so \
	../LuaJIT-2.1.0-beta3/src/libluajit.a \
	../jansson-2.10/src/.libs/libjansson.so \
	./libs/dt.so \
	-lm  -lpthread -ldl 

clean:
	rm -r -f dt_httpd $(COBJS) 
	make -i -C ../LuaJIT-2.1.0-beta3/src/ clean 
	make -i -C ../libevent-2.0.22-stable/ distclean 
	make -C ../dt_intepreter/src clean
	rm -f ../dt_interpreter/src/*.so 
	rm -f ../USER_AGENT_CLASSIFIER/src/*.so 
	rm -r -f ../jansson-2.10/* # TODO P3 Check with Indrajeet
	echo "ALL CLEAN"

qclean :
	rm -r -f ab_httpd $(COBJS) *.o _*
	rm -f libs/*
	rm -f dt_httpd
