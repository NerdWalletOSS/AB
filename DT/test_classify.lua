local curl   = require 'lua/curl'
local JSON   = require 'lua/JSON'
local cmp    = require 'DT/compare_predictions'
local plpath = require 'pl.path'
local plfile = require 'pl.file'

local url = "localhost:8000/Classify"
assert(#arg == 2, "Usage is lua " .. arg[0] .. " <infile> <opfile> ")
infile = arg[1]
opfile = arg[2]
assert(plpath.isfile(infile))
assert(plpath.isfile(opfile))

local invals = assert(plfile.read(infile))
local opvals = assert(plfile.read(opfile))
invals = assert(JSON:decode(invals))
opvals = assert(JSON:decode(opvals))
local n_in = 0
local n_op = 0
for k, v in pairs(invals) do n_in = n_in + 1 end 
for k, v in pairs(opvals) do n_op = n_op + 1 end 
assert(n_in == n_op)
assert(n_in > 0)
for k, v in pairs(invals) do 
  local str = assert(JSON:encode(v))
  -- print(str)
  local hdrs, body, status = curl.post(url, nil, str)
  assert(status == 200 )
  -- print(body)
  assert(cmp(body, body))
end




print("SUCCESS")
