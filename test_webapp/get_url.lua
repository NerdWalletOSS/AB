local cURL = require "cURL"
local get_hdrs, reset_hdrs, return_hdrs 
local x, y, z = require('get_hdrs')
local get_hdrs    = x[1]
local reset_hdrs  = x[2]
local return_hdrs = x[3]
local x = require('get_body')
local get_body    = x[1]
local reset_body  = x[2]
local return_body = x[3]

local function get_url(
  url
  )
  curl_params = { 
    url        = url,
    post       = false,
    writefunction  = get_body,
    headerfunction = get_hdrs,
  }
  reset_hdrs(); reset_body()
  local c = cURL.easy(curl_params)
  local x = c:perform()
  local c = cURL.easy(curl_params)
  return return_body(), return_hdrs()
end
return get_url

--[[
url = "localhost:8080/AB/php/endpoints/endpoint_test_info.php?TestID=1"
url = "localhost:8080/AB/test_webapp/landing_page_0.php"
x, y = get_url(url)
print(x, y)
--]]
