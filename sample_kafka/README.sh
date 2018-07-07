cd ../bin/libs/
export LD_LIBRARY_PATH="`pwd`"
cd -
gcc sample.c ../librdkafka/src/librdkafka.so.1 -I../librdkafka/src
