local JSON = require 'lua/JSON'
local plfile = require 'pl.file'
local plpath = require 'pl.path'

local threshold = 0.001
local epsilon   = 0.001

assert( type(arg) == "table")
local file1 = arg[1]
local file2 = arg[2]
assert(plpath.isfile(file1))
assert(plpath.isfile(file2))
local P1 = JSON:decode(plfile.read(file1))
local P2 = JSON:decode(plfile.read(file2))
for k1, v1 in pairs(P1) do 
  v2  = assert(P2[k1])
  diff = v1 - v2
  denom = v1 + v2
  if ( diff < 0 ) then diff = diff * -1 end
  if ( denom >= epsilon ) then
    assert( (diff / denom ) < threshold, 
      "k1 = " .. k1 .. " v1 = " .. v1 .. " v2 = " .. v2 )
  end
end
print("Predictions match")
