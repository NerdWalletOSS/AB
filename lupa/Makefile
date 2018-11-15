#
# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
WARN=  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic -Wno-aggregate-return -Wno-unused-parameter
INCS= -I. \
	-I../LuaJIT-2.1.0-beta3/src/ \
	-I../dt_interpreter/src/  \
	-I../dt_server/  \

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

all : lupa_dt.so 

./libs:
	mkdir libs

../LuaJIT-2.1.0-beta3/src/libluajit.a:
		cd ../ && \
		rm -rf LuaJIT-2.1.0-beta3 && \
	 	tar -xvzf LuaJIT-2.1.0-beta3.tar.gz && \
	 	cd LuaJIT-2.1.0-beta3/ && \
		sed -i '114s/#//' src/Makefile && \
		make TARGET_FLAGS=-pthread

.c.o : 
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS) $(DBGFLAGS)

../lua-5.1.3/src/liblua.a :
	make -C ../lua-5.1.3/src/ linux

$(COBJS) : ../LuaJIT-2.1.0-beta3/src/libluajit.a 

../dt_interpreter/src/dt.so : 
	make -C ../dt_interpreter/src/

lupa_dt.so : $(COBJS)  \
	../LuaJIT-2.1.0-beta3/src/libluajit.a  \
	../dt_interpreter/src/dt.so 
	$(CC) -shared $(CFLAGS) $(COBJS) \
	  ../dt_interpreter/src/dt.so \
	  -o lupa_dt.so  \
	../LuaJIT-2.1.0-beta3/src/libluajit.a \
	-lm  -lpthread -ldl 

clean:
	make -i -C ../LuaJIT-2.1.0-beta3/src/ clean 
	make -C ../dt_interpreter/src clean
	echo "ALL CLEAN"

qclean :
	rm -r -f *.o *.so
