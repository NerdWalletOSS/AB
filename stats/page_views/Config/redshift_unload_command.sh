#!/bin/bash
source ./Config/environment.ctrl
input=`cat $1`
query="unload "$input" to 's3://nerdwallet-dwh-redshift"$2$3"' credentials 'aws_access_key_id="$s3_access_key";aws_secret_access_key="$s3_secret_key"' delimiter ',' allowoverwrite parallel off;"
psql -h "$pdbHost" -p "$pport" -U "$pusername" -d "$pdatabase"  -c "$query"
