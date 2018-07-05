#!?bin/bash
EXTRA_FLAGS=$(shell cat ../../conanbuildinfo.gcc)
WARN=-std=gnu99 -Wall -fPIC -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic

.DELETE_ON_ERROR: ua_to_dev.bin

all: ut process_user_agents

rs_tmp_user_agent000:
	bash export.sh

.c.o : 
	$(CC) -c -o $@ $< $(CFLAGS)  $(INCS)

clean:
	rm -f process_user_agents ua_to_dev.bin rs_tmp_user_agent000

qclean:
	rm -f process_user_agents ua_to_dev.bin 

process_user_agents:
	gcc -o process_user_agents -g ${WARN} ${EXTRA_FLAGS} -I../  \
	   process_user_agents.c \
	  ../spooky_hash.c  \
	  ../load_lkp.c  \
	  ../auxil.c \
	  ../mmap.c

ut : 
	gcc -g -DLOCAL_DEPLOY ${WARN} -I../ \
	cli_classify_ua.c  \
	../spooky_hash.c  \
	../load_lkp.c  \
	../url.c  \
	../load_classify_ua_map.c  \
	../classify_ua.c  \
	../auxil.c \
	../mmap.c \
	-o ut

ua_to_dev.bin: process_user_agents rs_tmp_user_agent000 
	./process_user_agents rs_tmp_user_agent000 ua_to_dev.bin

install: ua_to_dev.bin device.csv
	cp ua_to_dev.bin /opt/ab/ua_to_dev.bin
	cp device.csv /opt/ab/device.csv
