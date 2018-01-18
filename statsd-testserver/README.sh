sudo apt-get install python python-pip -y
pip install pystatsd
python -c 'import pystatsd; pystatsd.Server(debug=True).serve()' & 
gcc -I ../statsd-c-client-master/ test.c ../statsd-c-client-master/libstatsdclient.so
./a.out
sleep 10
kill -9 %1
rm ./a.out
