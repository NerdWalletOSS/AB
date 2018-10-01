#!/bin/bash
source ./Config/environment.ctrl
query=`cat $1`
psql -h "$pdbHost" -p "$pport" -U "$pusername" -d "$pdatabase"  -c "$query"
