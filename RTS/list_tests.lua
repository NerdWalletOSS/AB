local cache = require 'lua/cache'
local JSON  = require 'lua/JSON'

return function()
  local tests = cache.get('tests')
  local o_table = {}
  if ( not tests ) then return " [] " end
  for _,v in pairs(tests) do
    local entry = {}
    entry.name = v.name
    entry.TestType = v.TestType
    o_table[#o_table + 1] = entry
  end
  return JSON:encode(o_table)
end
