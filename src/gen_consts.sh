#!/bin/bash

echo "local consts = {}" >../lua/ab_consts.lua
cat ab_constants.h |grep "#define" | grep -v "__" | awk '{print $2"="$3}' | sed 's/\([a-zA-Z_]\+\)/consts\.\1/g' >> ../lua/ab_consts.lua
echo "return consts" >> ../lua/ab_consts.lua
