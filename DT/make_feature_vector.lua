local ffi              = require "ffi"
local JSON             = require "lua/JSON"

local function make_feature_vector(
  fvec_as_json, 
  fvec, -- feature vector , C array [n_fvec] */
  n_fvec -- number of elements in C feature vector 
  )
  local fvec_tbl = assert(JSON:decode(fvec_as_json), 
    "Feature vector not valid JSON")
  assert(fvec)
  fvec = ffi.cast("float*", fvec)

  assert(n_fvec)
  assert(type(n_fvec) == "number")
  assert(n_fvec > 0)
  -- set up an array to check you have values for all features
  local fidx_seen = {}
  for i = 1, n_fvec do 
    fidx_seen[i] = false
  end
  --=================================
  for k, v in pairs(dt_feature_to_idx) do print(k, v) end 
  --=================================
  for k, v in pairs(fvec_tbl) do
    print(k, v)
    local fidx = assert(dt_feature_to_idx[k])
    assert( ( fidx >=1 ) and ( fidx <= n_fvec ) )
    assert(type(v) == "number")
    fidx_seen[fidx] = true
    fvec[fidx-1] = v -- -1 because C is indexed from 0, Lua from 1
  end
  -- check that you saw all features
  for i = 1, n_fvec do 
    -- print(i, fidx_seen[i])
    assert(fidx_seen[i]) 
  end
  return true
end
return make_feature_vector
