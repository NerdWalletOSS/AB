make clean
make
dtfile=../data/dt2.csv
test -f $dtfile
testfile=../data/test1.csv
test -f $testfile
outfile=dt.bin
./test_dt $dtfile $testfile $outfile
test -d $/opt/ab/
cp $dtfile /opt/ab/
echo "Successfully completed $0 in $PWD"
