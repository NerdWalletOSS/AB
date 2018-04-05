local ffi = require 'ffi'
local assertx = require 'assertx'
local cache = require 'cache'
local get_n_cards = require 'get_n_cards'

local function return_output(
  opvec, -- feature vector, now a float* of n_opvec length
  n_opvec, -- number of elements in feature vector. should be 21
  out_buf, -- output buffer for debugging 
  sz_out_buf -- size of above
  )
  opvec = ffi.cast("float*", opvec)
  local out_features = {}
  -- read the g_ccid_mapping
  local g_ccid_mapping = assert(cache.get('g_ccid_mapping'),
    'G_CCID_MAPPING missing from cache.')
  local n_cards = assert(get_n_cards(), 'get_n_cards failed.')
  assertx(n_opvec == n_cards, 'feature vector length is ,',
    tostring(n_opvec), ', but g_ccid_mapping says length is ', 
    tostring(n_cards))
  for idx, ccid in pairs(g_ccid_mapping) do
    local ccid = assert(tonumber(ccid), 'g_ccid_mapping faulty')
    local pred = assertx(opvec[tonumber(idx)], 'Index ', tostring(idx),
      ' not present in opvec.')
    assertx(0 <= pred and pred <= 1, 'prediction ', tostring(pred), 
      ' not between 0 and 1 inclusive.')
    out_features[ccid] = pred
  end
  local x = assert(json.encode(out_features))
  assertx(#x <= sz_out_buf, 'len of string is ', #x,
    ', which is too long for the buffer with length ', sz_out_buf)
  ffi.copy(out_buf, x)
end

return return_output