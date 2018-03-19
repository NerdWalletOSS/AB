local ffi = require 'ffi'
local plpath = require 'pl.path'
local plfile = require 'pl.file'

function include(filename)
  assert(plpath.isfile(filename), "File not found " .. filename)
  plfile.delete("_temp.h")
  os.execute(' grep -v "^#" ' .. filename .. ' > _temp.h')
  assert(plpath.isfile("_temp.h"))
  local x = assert(plfile.read('_temp.h'))
  ffi.cdef(x)
  plfile.delete("_temp.h")
end


include("rfr_types.h")
include("strip_url.h")
local libh = ffi.load("./referer.so")
ffi.cdef("void *malloc(size_t size);")
ffi.cdef("void *memset(void *s, int c, size_t n);")
ffi.cdef("char *strcpy(char *dest, const char *src);")
ffi.cdef(" size_t strlen(const char *s);")

-- local input = ffi.C.malloc(ffi.sizeof("STR_REC_TYPE"))
local input = ffi.C.malloc(ffi.sizeof("double"))
input = ffi.cast("STR_REC_TYPE *", input)

local sz = 1024
input[0].X = ffi.C.malloc(sz)
ffi.C.memset(input[0].X, 0, sz)
local instr = "/junk/avx/def+"
ffi.C.strcpy(input[0].X, instr)
input[0].nX = ffi.C.strlen(instr)

output = ffi.cast("STR_REC_TYPE *", ffi.C.malloc(ffi.sizeof("STR_REC_TYPE")))

output[0].X = nil
output[0].nX = 0

libh.strip_url(input[0], output);
local outstr = ffi.string(output.X, output.nX)
assert(outstr == "junk/avx/def")
