-- Only does PST and GET

package.path=package.path .. ";./../src/?.lua;./../test_integration/?.lua;../test_generator/?.lua"
-- require 'strict'
require 'str'
local JSON = require 'json' -- one-time load of the routines
local cURL = require "cURL"
local ltn12 = require("ltn12") -- TODO What is this for?
-- local dbg = require 'debugger'
local function trim1(s)
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end
local res_hdrs = {}
local function add_to_hdrs(str)
  res_hdrs[#res_hdrs+1] = trim1(str)
end

local res_body = ""
local function add_to_body(str)
  res_body[#res_body +1] =  str
end

local function init_res_fields()
  res_body = {}
  res_hdrs = {}
end

local curl = {}

function curl.get(c_url, hdrs, body)
  init_res_fields()
  assert(type(c_url) == "string" and #c_url >0 , "Url must be a valid string")
  assert(hdrs == nil or type(hdrs) == "table" , "Headers if present should be a table")
  local request = {
    url = c_url,
    httpheader = hdrs,
    writefunction = add_to_body,
    headerfunction = add_to_hdrs,
  }
  if body ~= nil then
    request.postfields = JSON.encode(body)
  end
  local curl_handle = cURL.easy(request)
  local result = curl_handle:perform()
  -- return of previous is same as curl handle
  curl_handle:close()
  local return_code = tonumber(res_hdrs[1]:split(" ")[2])
  assert(return_code, "Invalid return code " .. res_hdrs[1]:split(" ")[2])
  res_body = table.concat(res_body, "")
  return res_hdrs, res_body, return_code
end

function curl.post(c_url, hdrs, body)
  init_res_fields()
  assert(type(c_url) == "string" and #c_url >0 , "Url must be a valid string")
  assert(hdrs == nil or type(hdrs) == "table" , "Headers if present should be a table")
  local request = {
    url = c_url,
    post = true,
    httpheader = hdrs,
    writefunction = add_to_body,
    headerfunction = add_to_hdrs,
  }
  if body ~= nil then
    request.postfields = JSON.encode(body)
  end
  local curl_handle = cURL.easy(request)
  local result = curl_handle:perform()
  -- return of previous is same as curl handle
  curl_handle:close()
  local return_code = tonumber(res_hdrs[1]:split(" ")[2])
  assert(return_code, "Invalid return code " .. res_hdrs[1]:split(" ")[2])
  res_body = table.concat(res_body, "")
  return res_hdrs, res_body, return_code
end


-- print("Results", curl.get("http://192.168.56.105:8080/AB/index.html"))
-- print("Results", curl.get("http://192.168.56.105:8080/index.tml"))
-- print("Results", curl.post("http://192.168.56.105:8080/AB/index.tml", nil, "baklava"))

return curl
-- pattern to get return code from header is res_hdrs[1]:split(" ")[2]
