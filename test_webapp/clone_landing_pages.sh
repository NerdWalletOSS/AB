#!/bin/bash
infile=landing_page_0.php
test -f $infile
iter=1
while [ $iter -le 8 ]; do 
  cat $infile | sed s"/ 0/ $iter"/g > landing_page_$iter.php
  iter=`expr $iter + 1`
done
