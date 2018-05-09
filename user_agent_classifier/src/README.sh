#!/bin/bash
set -e

make clean
make
DIR=../data/
test -d $DIR
./test_ua \
  $DIR/category_intercept_1.csv  \
  $DIR/model_coefficients_1.csv \
  $DIR/input_1.csv > _output_1.csv 
lua compare.lua
echo "Successfully completed $0 in $PWD"
rm -f _*
