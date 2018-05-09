package.path = package.path .. ";./../src/?.lua;./../test_webapp/?.lua;../test_generator/?.lua"
require 'str'
local JSON = require 'JSON'
local plfile = require 'pl.file'
local plpath = require 'pl.path'
local curl = require 'curl'
--==========================
local function mk_test(x)
  local T 
  if ( type(x) == "table" ) then
    T = x
  elseif ( type(x) == "string" ) then  
    assert(plpath.isfile(infile) )
    T = dofile(infile)
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
