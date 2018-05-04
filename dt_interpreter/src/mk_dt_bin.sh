make clean
make
dtfile=../data/spam_data/spam_dt.csv
test -f $dtfile
testfile=../data/spam_data/test_1_10.csv
test -f $testfile
dtfile=dt.bin
rffile=rf.bin
mdlfile=mdl.bin
./test_dt $dtfile $testfile $dtfile $rffile $mdlfile
test -d $/opt/ab/
cp $dtfile /opt/ab/
cp $rffile /opt/ab/
cp $mdlfile /opt/ab/
echo "Successfully completed $0 in $PWD"
