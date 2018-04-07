local json = require 'json'
local generate_features = require 'generate_features'
local one_hot_encoding = require 'one_hot_encoding'
local get_num_features = require 'get_num_features'
local ffi = require 'ffi'
local assertx = require 'assertx'

local function fake_make_feature_vector(str_feature_vector)
  local feature_table = json.decode(str_feature_vector)
  -- does the conversions
  feature_table = generate_features(feature_table)
  local ohe = one_hot_encoding(feature_table)
  local n_features = get_num_features()
  -- for debugging
  for k, v in pairs(ohe) do print(k, v) end
  for k, v in pairs(feature_table) do print(k, v) end
  -- end debugging
  -- init feature_vector
  local feature_vector = {}
  for i=0, n_features-1 do
    feature_vector[i] = 0
  end
  -- fill feature vector
  for k, v in pairs(ohe) do
    assertx(n_features >= k, 'Index of ', tostring(k), ' too big for g_dt_feature_vector, which has max ', tostring(n_features), ' entries.')
    feature_vector[k - 1] = v
  end
  -- verify feature_vector
  for i=0,n_features-1 do print(i, feature_vector[i]) end
end


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
