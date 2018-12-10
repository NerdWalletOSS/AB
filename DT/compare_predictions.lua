local JSON = require 'lua/JSON'
local plfile = require 'pl.file'
local plpath = require 'pl.path'

local function compare_predictions(str1, str2, threshold, epsilon)
  if ( not threshold ) then threshold = 0.001 end 
  if ( not epslion ) then epsilon   = 0.001 end
  
  local P1 = assert(JSON:decode(str1))
  local P2 = assert(JSON:decode(str2))
  for k1, v1 in pairs(P1) do 
    v2  = assert(P2[k1])
    diff = v1 - v2
    denom = v1 + v2
    if ( diff < 0 ) then diff = diff * -1 end
    if ( denom >= epsilon ) then
      if ( (diff / denom ) > threshold ) then return false end
    end
  end
  return true
end
