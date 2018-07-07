require 'lua/str'
local reset_db  = require 'test_webapp/reset_db'
local curl      = require 'lua/curl'
local JSON      = require 'lua/JSON'

local url = 
  "http://localhost:8080/AB/php/endpoints/endpoint_add_admin_channel.php"
local tests = {}
tests.t1 = function ()
  local tables = { "admin", "channel" }
  reset_db()
  for _, tbl in ipairs(tables) do 
    local T = {}
    T.Table = tbl
    T.Name  = "joeblow"
    -- first add should succeed
    local a, b, c = curl.get(url, nil, JSON:encode(T))
    assert(c == 200)
    -- second add with same name should fail
    local a, b, c = curl.get(url, nil, JSON:encode(T))
    assert(c ~= 200)
  end
  print("Test t1 succeeded")
end
tests.t1() -- uncomment for quick and dirty testing
return tests
