#!/bin/bash
# This is only for local testing, not for use in production
# Make sure ab_httpd and ab_logger are running 
set +e
curl --url "localhost:8004/Halt"
logfile=/tmp/log_monitor.csv
rm -f $logfile
curl --url "localhost:8000/Halt"
set -e 
#---------------------------------------------------
mysql -u root < ../sql/create_table.sql
../src/ab_logger 8004 & 
sleep 1
../src/ab_httpd ../src/ab.test.conf &
sleep 1
curl -s --url "localhost:8004/SetVerbose?true"
curl -s --url "localhost:8004/SetOutput?Action=Start&LogFile=$logfile"
echo "Silencing ....."
# curl -s --url "localhost:8000/SetVerbose?Verbose=false"
echo "Starting ....."
iter=1
while [ $iter -lt 100 ]; do 
  echo "Iter $iter ";
  php cli_monitor.php
  x=`curl -s --url "localhost:8000/PerfMetrics"`; echo $x
  x=`curl -s --url "localhost:8004/PerfMetrics"`; echo $x
  iter=`expr $iter + 1`
done
