#/bin/bash
# This is a test script to verify the exact match user agent classifier
set -e 
make clean
make
VG=""
VG="valgrind --leak-check=full "
uafile=../data/rs_tmp_user_agent000
opfile=_ua_to_dev_map.bin
test -f $uafile
$VG ./mk_ua_to_dev_map $uafile $opfile  1>_out 2>&1
test -f $opfile
grep "0 errors" _out  1>/dev/null 2>&1
echo "Successfully completed $0 in $PWD"
