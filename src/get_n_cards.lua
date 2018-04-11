local cache = require 'cache'

local function get_n_cards()
  local ccid_mapping = assert(cache.get("ccid_mapping"),
  	"CCID_MAPPING not in cache")
  assert(ccid_mapping, 'ccid_mapping not loaded.')
  assert(ccid_mapping[0], 'ccid_mapping must be 0-indexed')
  local count = 0
  for k, v in pairs(ccid_mapping) do count = count + 1 end
  return count
end

return get_n_cards
