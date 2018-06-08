#
# For fast version, make -e DBGFLAGS="" -e OPTFLAGS=" -O4"
WARN=  -std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic -Wno-aggregate-return
INCS= -I. \
	-I../src/\
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
	-I../librdkafka/src/ \
	-I../USER_AGENT_CLASSIFIER/src/

DBGFLAGS=
DBGFLAGS= -g -DDEBUG

OPTFLAGS= -O4
OPTFLAGS=

CC= clang
CC= gcc

CSRCS := $(wildcard *.c)
COBJS = $(CSRCS:.c=.o)

all : kafka_producer

../librdkafka/src/librdkafka.so:
	cd ../ && rm -rf librdkafka &&  tar -xvzf librdkafka.tar.gz && cd librdkafka && ./configure && make

kafka_producer: ../librdkafka/src/librdkafka.so
	gcc ../src/kafka_*.c kafka_producer.c $(CFLAGS) $(INCS) -o kafka_producer ../librdkafka/src/librdkafka.so -ldl
