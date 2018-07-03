#!/bin/bash
set -e 
if [ $# != 0 ]; then echo "Usage is $0"; exit 1; fi
DIR=../../db_dump
php $DIR/from_redshift.php $DIR/config.json  user_agent
#------
echo "Successfully completed $0 in $PWD"
