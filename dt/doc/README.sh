#!/bin/bash
set -e
touch .meta
eval ` ../../doc/latex/tools/setenv`
make -f ../../doc/latex/tools/docdir.mk dt.pdf
