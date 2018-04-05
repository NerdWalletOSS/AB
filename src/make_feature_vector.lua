local json = require 'json'
local generate_features = require 'generate_features'
local one_hot_encoding = require 'one_hot_encoding'
local get_num_features = require 'get_num_features'
local ffi = require 'ffi'
local assertx = require 'assertx'


local function make_feature_vector(
  body, 
  fvec, -- feature vector 
  n_fvec, -- number of elements in feature vector 
  out_buf, -- output buffer for debugging 
  sz_out_buf, -- size of above
  is_debug -- boolean indicating whether to debug or not
  )
  local in_features = assert(json.decode(body), 
    "did not get valid JSON input")
  -- does the conversions
  local out_features = assert(generate_features(in_features))
  if ( is_debug ) then 
    local x = assert(json.encode(out_features))
    ffi.copy(out_buf, x)
    -- TODO P2 we need to make sure we do not overflow the buffer
  end
  local ohe = one_hot_encoding(out_features)
  local n_features = get_num_features()
  assertx(n_fvec == n_features, 
    "C's provided g_n_dt_feature_vector value of ", 
    n_fvec, 
    ", does not match with Lua's value of ", n_features)
  -- for debugging
  for k, v in pairs(ohe) do print(k, v) end
  -- end debugging
  fvec = ffi.cast("float*", fvec)
  ffi.fill(fvec, ffi.sizeof("float") * n_features) -- set to 0
  for k, v in pairs(ohe) do
    assertx(n_features >= k, 
    'Index of ', tostring(k), 
    ' too big for g_dt_feature_vector, which has max ', 
    tostring(n_features), ' entries.')
    fvec[k-1] = v
  end
end

return make_feature_vector
