package.path = '../../?.lua;../?.lua;../lua/?.lua;../../lua/?.lua;' .. package.path

local cache = require 'cache'
local ffi = require 'ffi'
local assertx = require 'lua.assertx'
local json = require 'lua.json'


local function get_mdl_ct(mdl_map)
  assert(mdl_map[0], 'mdl_map must be 0-indexed')
  local count = 0
  for k, v in pairs(mdl_map) do count = count + 1 end
  return count
end


local function post_proc_preds(
  opvec, -- feature vector, now a float* of n_opvec length
  n_opvec, -- number of elements in feature vector. should be 21
  out_buf, -- output buffer for debugging 
  sz_out_buf -- size of above
  )
  opvec = ffi.cast("float*", opvec)
  local out_features = {}
  local mdl_map = assert(cache.get('mdl_map'),
    'mdl_map missing from cache.')
  local n_mdl = assert(get_mdl_ct(mdl_map), 'mdl_map failed.')
  assertx(n_opvec == n_mdl, 'feature vector length is ',
    tostring(n_opvec), ', but mdl_map says length is ', 
    tostring(n_mdl))
  for idx, mdl in pairs(mdl_map) do
    mdl_n = assert(tonumber(mdl), 'mdl_map faulty')
    local pred = assertx(opvec[tonumber(idx)], 'Index ', tostring(idx),
      ' not present in opvec.')
    assertx(0 <= pred and pred <= 1, 'prediction ', tostring(pred), 
      ' not between 0 and 1 inclusive.')
    out_features[mdl_n] = pred
  end
  local x = assert(json.encode(out_features))
  assertx(#x <= sz_out_buf, 'len of string is ', #x,
    ', which is too long for the buffer with length ', sz_out_buf)
  ffi.copy(out_buf, x)
end

return post_proc_preds
