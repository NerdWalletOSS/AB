local curl = require 'lua/curl'
local JSON = require 'lua/JSON'
local lt_url =  -- list tests URL
 "http://localhost:8080/AB/php/endpoints/endpoint_list_tests.php"

local function get_tests(test_type)
  local B = {}
  B.TestType = "all"
  local h, b, c = curl.get(lt_url, nil, JSON:encode(B))
  assert(c == 200)
  local T = {}
  for k, v in pairs(JSON:decode(b) ) do
    local t = {}
    t.name = v.name
    t.State = v.State
    t.TestType = assert(v.TestType)
    T[v.id] = t
  end
  return T
  
end
return get_tests
-- local T = assert(get_tests())
