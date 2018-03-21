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


include("../src/utm_types.h")
include("../src/utm_kv.h")
local libh = ffi.load("../src/utm_kv.so")
ffi.cdef("void *malloc(size_t size);")
ffi.cdef("void free(void *);")
ffi.cdef("void *memset(void *s, int c, size_t n);")
ffi.cdef("char *strcpy(char *dest, const char *src);")
ffi.cdef(" size_t strlen(const char *s);")

local instr = [[
https://www.nerdwallet.com/l/banking/best-cd-rates?nw_campaign_id=150845972646889300&utm_source=goog&utm_medium=cpc&utm_campaign=bk_mktg_paid_121417_beta_CDs&utm_term=%2Bcd&utm_content=ta&mktg_hline=7517&mktg_body=1699&mktg_place=5&gclid=Cj0KCQiAzMDTBRDDARIsABX4AWzrC8Y_TgM6-tfQomcrrVrI21SFPj946phFQQtsJX_8l3xiIwBPpZEaAgCPEALw_wcB&gclsrc=aw.ds&VariantID=3334&GUID=110043&DeviceID=0
]]

output = ffi.cast("UTM_REC_TYPE *", ffi.C.malloc(ffi.sizeof("UTM_REC_TYPE")))

libh.utm_kv(instr, output);
assert(ffi.string(output.medium) == "cpc")
assert(ffi.string(output.source) == "goog")
assert(ffi.string(output.campaign) == "bk_mktg_paid_121417_beta_cds")


assert(nil, "PREMATURE")
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
