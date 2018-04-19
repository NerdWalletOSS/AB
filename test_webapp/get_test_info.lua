package.path = package.path .. ";./../src/?.lua;./../test_webapp/?.lua;../test_generator/?.lua"
require 'str'
local json = require 'json'
local plfile = require 'pl.file'
local plpath = require 'pl.path'
local curl = require 'curl'
--==========================
local function get_test_info(
  test_id
  )
  assert(type(test_id) == "number")
  local url = "http://localhost:8080/AB/php/endpoints/endpoint_test_info.php?TestID=" .. test_id
  local hdrs, body, status = curl.post(url)
  assert(status == 200)
  return hdrs, body
end
return get_test_info
