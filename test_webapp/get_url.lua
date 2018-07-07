require 'strict'
local cURL = require "cURL"

local xhdrs = {}
local function trim1(s)
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end
local function xget_hdrs (str)
  xhdrs[#xhdrs+1] = trim1(str)
end
local function xreset_hdrs ()
  xhdrs = {}
  xhdrs[1] = "DEF"
end
local function xreturn_hdrs ()
  return xhdrs
end
local xbody = ""
local function xget_body (str)
  xbody = str
end
local function xreset_body ()
  xbody = ""
end
local function xreturn_body ()
  return xbody
end

local function get_url(
  url
  )
  local xcurl_params = { 
    url        = url,
    post       = false,
    writefunction  = xget_body,
    headerfunction = xget_hdrs,
  }
  xreset_hdrs(); xreset_body()
  local c = cURL.easy(xcurl_params)
  local x = c:perform()
  local c = cURL.easy(curl_params)
  return xreturn_body(), xreturn_hdrs()
end
return get_url

--[[
url = "localhost:8080/AB/php/endpoints/endpoint_test_info.php?TestID=1"
url = "localhost:8080/AB/test_webapp/landing_page_0.php"
x, y = get_url(url)
print(x, y)
--]]
