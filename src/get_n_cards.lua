local cache = require 'cache'
--local ccid_mapping = '../dt_interpreter/data/ccid_mapping.lua'

local function get_n_cards()
  local g_ccid_mapping = assert(cache.get("g_ccid_mapping"),
  	"CCID_MAPPING not in cache")
  assert(g_ccid_mapping, 'g_ccid_mapping not loaded.')
  assert(g_ccid_mapping[0], 'g_ccid_mapping must be 0-indexed')
  local count = 0
  for k, v in pairs(g_ccid_mapping) do count = count + 1 end
  return count
end

return get_n_cards
