require 'lua/str'
local plpath = require 'pl.path'
local JSON = require 'lua/JSON'
local curl = require 'lua/curl'
--==========================
local function mk_test(x)
  local T 
  if ( type(x) == "table" ) then
    T = x
  elseif ( type(x) == "string" ) then  
    assert(plpath.isfile(x) )
    T = dofile(x)
  else
    assert(nil, "bad type of input to mk_test")
  end
  assert(T)
  local inbody = JSON:encode(T)
  local url = "localhost:8080/AB/php/endpoints/endpoint_test_basic.php"
  local hdrs, outbody, status = curl.post(url, nil, inbody)
  return hdrs, outbody, status
end
return mk_test
-- mk_test("good_basic1.lua")
