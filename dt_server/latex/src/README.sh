#!/bin/bash
set -e
touch .meta
rm -f dt_server.pdf
eval `../../../doc/latex/tools/setenv`
make -f ../../../doc/latex/tools/docdir.mk dt_server.pdf
cp dt_server.pdf /tmp/
echo "File created"
