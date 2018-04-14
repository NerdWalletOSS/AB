#!?bin/bash
set -e
mapfile=/opt/ab/ua_to_dev.bin
test -f $mapfile
test -f ut
while read ua 
do 
  echo "Testing $ua"
  ./ut $mapfile "$ua"
  echo ""
done < sample_user_agent.csv
echo "Successfully completed $0 in $PWD"
