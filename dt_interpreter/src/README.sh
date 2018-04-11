make clean
make
dtfile=../data/dt2.csv
test -f $dtfile
testfile=../data/test1.csv
test -f $testfile
dtfile=dt.bin
rffile=rf.bin
mdlfile=mdl.bin
./test_dt $dtfile $testfile $dtfile $rffile $mdlfile
test -d $/opt/ab/
cp $dtfile /opt/ab/
echo "Successfully completed $0 in $PWD"
