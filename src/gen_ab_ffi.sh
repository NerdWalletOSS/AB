#!/bin/bash
# gen the doth with the #defines replaced
echo '#include "ab_constants.h"' > ffi.h
cat ab_types.h | grep -v "^#" >> ffi.h
echo "local ffi = require 'ffi'"  > ab_ffi.lua
echo "ffi.cdef([[" >>ab_ffi.lua
gcc -E ffi.h | grep -v "^#">>ab_ffi.lua
echo "]])" >>ab_ffi.lua
echo "return ffi" >>ab_ffi.lua
rm ffi.h
