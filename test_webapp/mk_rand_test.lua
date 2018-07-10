require 'lua/str'
local JSON    = require 'lua/JSON'
local curl    = require 'lua/curl'
local tbp_url = "localhost:8080/AB/php/endpoints/endpoint_test_basic.php"

local R = require 'test_webapp/rand_selections'

local function mk_rand_test(
  X
  )
  -- TODO Following is a hack. Needs to be done properly
  local T = {}
  if ( not X ) then X = {} end 
  T.name = X.name or tostring(math.random(1, 1000000000))
  local hdrs, outbody, status = curl.post(tbp_url, nil, JSON:encode(T))
  assert(status == 200 )
  return get_test_id(hdrs)
end
return mk_rand_test
