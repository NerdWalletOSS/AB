#!?bin/bash
EXTRA_FLAGS=$(shell cat ../../conanbuildinfo.gcc)
WARN=-std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic

.DELETE_ON_ERROR: ua_to_dev.bin

all: ua_to_dev.bin

rs_tmp_user_agent000:
	bash export.sh

clean:
	rm -f process_user_agents ua_to_dev.bin rs_tmp_user_agent000

process_user_agents:
	gcc -o process_user_agents -g ${WARN} ${EXTRA_FLAGS} \
		-I../ -I../../hhash -I../../statsd-c-client-master \
	   process_user_agents.c \
	  ../spooky_hash.c  \
	  ../load_device.c  \
	  ../auxil.c \
	  ../mmap.c

ua_to_dev.bin: process_user_agents rs_tmp_user_agent000
	./process_user_agents device.csv device_mapping.csv rs_tmp_user_agent000 ua_to_dev.bin

install: ua_to_dev.bin device.csv
	cp usa_to_dev.bin /opt/ab/ua_to_dev.bin
	cp device.csv /opt/ab/device.csv
