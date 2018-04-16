FLAGS="-std=gnu99 -g "
FLAGS="-std=gnu99 -O4 "
# Make sequential blaster
gcc $FLAGS sblaster.c \
  -std=gnu99 \
    -I../src/ -I$HOME/local/include/ \
    ../src/auxil.c \
    ../src/mmap.c \
    -o sblaster -lcurl
# Make parallel blaster (uses pthreads)
gcc $FLAGS pblaster.c \
  -std=gnu99 \
    -I../src/ -I$HOME/local/include/ \
    ../src/auxil.c \
    -o pblaster \
    -lpthread -lcurl

# I have an #define CREATE_TEST which needs to be set depending 
# on whether sblaster is supposed to create T1, T2,... or not
# Use 0 for logger port if you don't care about logging
echo "Started Sequential Blaster"
./sblaster localhost 8000 8004
# ./sblaster ab.nerdwallet.biz 80 0 
echo "Completed Sequential Blaster"

echo "Started Parallel Blaster"
./pblaster localhost 8000 8004
echo "Completed Parallel Blaster"

# echo PREMATURE; exit 1; 
# rm -f ./blaster 
