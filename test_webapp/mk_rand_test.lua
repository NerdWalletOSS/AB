package.path = package.path .. ";./../src/?.lua;./../test_webapp/?.lua;../test_generator/?.lua"
require 'str'
local JSON = require 'JSON'
local plfile = require 'pl.file'
local plpath = require 'pl.path'
local curl = require 'curl'
-- INDRAJEET TODO 
local function mk_rand_test(
  X
  )
  -- TODO Following is a hack. Needs to be done properly
  local T = {}
  local infile = "good_basic1.lua"
  T = assert(dofile(infile))
  local url = "localhost:8080/AB/php/endpoints/endpoint_test_basic.php"
  local hdrs, outbody, status = curl.post(url, nil, JSON:encode(T))
  return hdrs, outbody, status
end
return mk_rand_test
