local cache = require 'lua/cache'
local JSON  = require 'lua/JSON'

return function(args)
  local j_table = JSON:decode(args)
  local tests = cache.get('tests')
  local test = nil
  if j_table.id ~= nil then
    test = tests[j_table.id]
    assert(test ~= nil, "Tests with id ", j_table.id, " does not exist")
  elseif j_table.name ~= nil and j_table.test_type ~= nil then
    for _, v in pairs(tests) do
      if v.name == j_table.name and v.test_type == j_table.test_type then
        assert(test == nil, "More than one match exists")
        test = v
      end
    end
  end
  return JSON:encode(test)
end

