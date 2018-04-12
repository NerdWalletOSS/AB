#!/bin/bash
iter=1
while [ $iter -le 8 ]; do 
  echo "<html><body><h1>It really works number $iter!</h1></body></html>" > ab_page_$iter.html
  iter=`expr $iter + 1`
done
