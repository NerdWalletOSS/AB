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


-- Now do it for a file
local in_file_name = "./strip/url_strip_output.txt"
local out_s = "/home/subramon/utm_source_out.txt"
local out_c = "/home/subramon/utm_campaign_out.txt"
local out_m = "/home/subramon/utm_medium_out.txt"

assert(plpath.isfile(in_file_name))
assert(plpath.isfile(out_s))
assert(plpath.isfile(out_c))
assert(plpath.isfile(out_m))

local ifh = assert(io.open(in_file_name, "r"))
local sfh = assert(io.open(out_s, "r"))
local mfh = assert(io.open(out_m, "r"))
local cfh = assert(io.open(out_c, "r"))
local num_errors = 0
local num_lines = 0
while true do 
  -- read string in 
  instr = ifh:read()
  if ( not instr ) then 
    print("Read lines " .. num_lines)
    break
  end
  -- TODO strip it and then call utm_kv
  --=================================
  libh.utm_kv(instr, output);
  local exp_sstr = sfh:read()
  local exp_mstr = mfh:read()
  local exp_cstr = cfh:read()
  if ( not exp_sstr ) then 
    print("Nothing more to compare against")
    break
  end
  if ( exp_sstr == nil) or ( exp_mstr == nil ) or ( exp_sstr == nil ) then 
    print("Nothing more to compare against") break
  end
  if ( exp_sstr == '""' ) then exp_sstr = '' end
  if ( exp_mstr == '""' ) then exp_mstr = '' end
  if ( exp_cstr == '""' ) then exp_cstr = '' end

  -- print(exp_sstr, exp_mstr, exp_cstr)
  local sstr = ffi.NULL
  local mstr = ffi.NULL
  local cstr = ffi.NULL
  if ( output[0].source ~= ffi.NULL ) then 
    sstr = ffi.string(output.source) 
  end 
  if ( output[0].medium ~= ffi.NULL ) then 
    mstr = ffi.string(output.medium) 
  end 
  if ( output[0].campaign ~= ffi.NULL ) then 
    cstr = ffi.string(output.campaign) 
  end
  if sstr and ( sstr ~= exp_sstr ) then 
    print("Source Line: " .. num_lines .. "{" .. sstr .. "}" .. "[" .. exp_sstr .. "]") 
    num_errors = num_errors + 1
  end
  if mstr and ( mstr ~= exp_mstr ) then 
    print("Medium Line: " .. num_lines .. "{" .. mstr .. "}" .. "[" .. exp_mstr .. "]") 
    num_errors = num_errors + 1
  end
  if cstr and ( cstr ~= exp_cstr ) then 
    print("Campaign Line: " .. num_lines .. "{" .. cstr .. "}" .. "[" .. exp_cstr .. "]") 
    num_errors = num_errors + 1
  end
  num_lines = num_lines + 1
  if ( num_errors >= 10 ) then break end 
  -- if ( num_lines == 100 ) then break end 
end -- iterate over all lines
print("num_errors = ", num_errors)
print("num_lines  = ", num_lines)
