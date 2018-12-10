local assertx  = require 'lua/assertx'
local cache    = require 'lua/cache'
local ffi      = require 'ffi'
local JSON     = require 'lua/JSON'

local function get_mdl_ct(mdl_map)
  assert(mdl_map[0], 'mdl_map must be 0-indexed')
  local count = 0
  for k, v in pairs(mdl_map) do count = count + 1 end
  return count
end

local function post_proc_preds(
  opvec, -- vector, now a float* of n_opvec length
  n_opvec -- number of elements in vector
  )
  assert(opvec)
  opvec = ffi.cast("float*", opvec)
  local out = {}
  for k, v in pairs(mdl_map) do 
    assert( ( k >=1 ) and ( k <= n_opvec ) )
    out[v] = opvec[k-1]
  end
  -- for k, v in pairs(out) do print(k, v) end
  -- TODO: do more error checking
  local x = assert(JSON:encode(out))
  return x
end
return post_proc_preds
