# following are one-time
sudo apt-get install python python-pip -y
pip install pystatsd
# above is one-time
# following is to start the server
python -c 'import pystatsd; pystatsd.Server(debug=True).serve()' & 
# following is to get a fake client going
gcc -I ../statsd-c-client-master/ test.c ../statsd-c-client-master/libstatsdclient.so
./a.out
sleep 10
kill -9 %1
rm ./a.out
