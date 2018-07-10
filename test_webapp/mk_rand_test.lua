require 'lua/str'
local JSON    = require 'lua/JSON'
local curl    = require 'lua/curl'
local tbp_url = "localhost:8080/AB/php/endpoints/endpoint_test_basic.php"
local R = require 'test_webapp/rand_selections'
local get_test_id = require 'test_webapp/get_test_id'

local function mk_rand_test(
  X
  )
  local T = R.rand_test(X)
  local hdrs, outbody, status = curl.post(tbp_url, nil, JSON:encode(T))
  assert(status == 200 )
  return get_test_id(hdrs)
end
return mk_rand_test
