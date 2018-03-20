local ffi = require 'ffi'
local plpath = require 'pl.path'
local plfile = require 'pl.file'
local stringio = require 'pl.stringio'

function include(filename)
  assert(plpath.isfile(filename), "File not found " .. filename)
  plfile.delete("_temp.h")
  os.execute(' grep -v "^#" ' .. filename .. ' > _temp.h')
  assert(plpath.isfile("_temp.h"))
  local x = assert(plfile.read('_temp.h'))
  ffi.cdef(x)
  plfile.delete("_temp.h")
end


include("../src/rfr_types.h")
include("../src/strip_url.h")
local libh = ffi.load("../src/referer.so")
ffi.cdef("void *malloc(size_t size);")
ffi.cdef("void free(void *);")
ffi.cdef("void *memset(void *s, int c, size_t n);")
ffi.cdef("char *strcpy(char *dest, const char *src);")
ffi.cdef(" size_t strlen(const char *s);")

-- local input = ffi.C.malloc(ffi.sizeof("STR_REC_TYPE"))
local input = ffi.C.malloc(ffi.sizeof("double"))
input = ffi.cast("STR_REC_TYPE *", input)

local sz = 4096
input[0].X = ffi.C.malloc(sz)
ffi.C.memset(input[0].X, 0, sz)
local instr = '"https://www.nerdwallet.com/investing/best-stock-broker-comparison/embed?brokerIds=31,2,35,19"'
ffi.C.strcpy(input[0].X, instr)
input[0].nX = ffi.C.strlen(instr)

output = ffi.cast("STR_REC_TYPE *", ffi.C.malloc(ffi.sizeof("STR_REC_TYPE")))

output[0].X = nil
output[0].nX = 0

libh.strip_url(input[0], output);
local outstr = ffi.string(output.X, output.nX)
assert(outstr == "https://www.nerdwallet.com/investing/best-stock-broker-comparison/embed?brokerIds=31,2,35,19")


-- Now do it for a file
local in_file_name = "./strip/url_strip_input.txt"
local out_file_name = "./strip/url_strip_output.txt"
assert(plpath.isfile(in_file_name))
assert(plpath.isfile(out_file_name))
local ifh = io.open(in_file_name, "r")
local ofh = io.open(out_file_name, "r")
local num_errors = 0
for i = 1, 1000000000 do 
  instr = ifh:read()
  if ( not instr ) then 
    print("Read lines " .. i)
    break
  end
  ffi.C.strcpy(input[0].X, instr)
  input[0].nX = ffi.C.strlen(instr)
  --=================================
  libh.strip_url(input[0], output);
  local outstr = ffi.string(output.X, output.nX)
  local expected_outstr = ofh:read()
  if ( outstr ~= expected_outstr ) then 
    print("input ", instr)
    print("actual ", outstr)
    print("expected ", expected_outstr)
    num_errors = num_errors + 1
  end
  -- assert(outstr == expected_outstr, "Failure at line " .. i)
end -- iterate over all lines
print("num_errors = ", num_errors)
