package.path = package.path .. ";./../src/?.lua;./../test_webapp/?.lua;../test_generator/?.lua"
require 'str'
local JSON = require 'JSON'
local plfile = require 'pl.file'
local plpath = require 'pl.path'
local cURL = require "cURL"
local mk_test = require 'mk_test'
local reset_db = require 'reset_db'
local get_test_id = require 'get_test_id'
local get_test_info = require 'get_test_info'
local get_error_code = require 'get_error_code'
local states = require 'states'
local function trim1(s)
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end
--============================
hdrs = {}
local function snarf_headers (str)
  hdrs[#hdrs+1] = trim1(str)
end
local function get_hdrs (str)
  hdrs[#hdrs+1] = trim1(str)
end
local function reset_hdrs ()
  hdrs = {}
  hdrs[1] = "abc"
end
local function return_hdrs ()
  return hdrs
end
--============================
body = ""
local function snarf_body (str)
  body = str
end
local function reset_body ()
  body = ""
end
local function return_body ()
  return body
end
--============================

-- START: Stuff common to all tests in this suite
  url = "localhost:8080/AB/php/endpoints/endpoint_test_basic.php"
  curl_params = { 
    url        = url,
    post       = true,
    httpheader = { "Content-Type: application/json"; },
    writefunction  = snarf_body,
    headerfunction = snarf_headers,
  }
addnl_curl_params = { 
  url = "localhost:8080/AB/php/endpoints/endpoint_set_state.php",
  post       = true,
  httpheader = { "Content-Type: application/json"; },
  writefunction  = snarf_body,
  headerfunction = snarf_headers,
}
-- STOP: Stuff common to all tests in this suite
local tests = {}
tests.t1 = function (
  just_pr
  )
  local description = "A basic test "
  if ( just_pr ) then print(description) return end

  reset_db()
  local T = {}
  T.name = "T1"

  T.TestType = "XYTest"
  T.Creator = "joe"
  T.id = ""
  --==============
  local v 
  local Variants = {}
  v = {} v.name = "Control" v.percentage = "50" v.url = "http://www.google.com"
  Variants[#Variants+1] = v
  v = {} v.name = "V1" v.percentage = "30" v.url = "http://www.yahoo.com"
  Variants[#Variants+1] = v
  v = {} v.name = "V2" v.percentage = "20" v.url = "http://www.cnn.com"
  Variants[#Variants+1] = v
  --==============
  T.Variants = Variants

  curl_params.postfields = JSON:encode(T)
  reset_hdrs(); reset_body()
  local c = cURL.easy(curl_params)
  local x = c:perform()
  local body = return_body(); local hdrs = return_hdrs()
--[[
  print("Start Headers")
  for k,v in pairs(hdrs) do print(k, v) end 
  print("Stop  Headers")
  ]]--


  local test_id = assert(get_test_id(hdrs))
  assert(test_id > 0)
  local error_code = assert(get_error_code(hdrs))
  assert(error_code == 200)
  --return T

  -- Create Another Test T2
  hdrs = {} -- IMPORTANT: Needs to be reset because set in t1()
  body = "" -- IMPORTANT: Needs to be reset because set in t1()
  T.name = "T2"
  curl_params.postfields = JSON:encode(T)
  local c = cURL.easy(curl_params)
  local x = c:perform()
  local test_id = assert(get_test_id(hdrs))
  assert(test_id > 0)
  local error_code = assert(get_error_code(hdrs))
  assert(error_code == 200)

-- SET STATE of T2 to dormant
  T.NewState = "dormant"
  hdrs = {} ; body = "" 
  addnl_curl_params.postfields = JSON:encode(T)
  local c = cURL.easy(addnl_curl_params)
  local x = c:perform()
  -- for k, v in pairs(hdrs) do print(k, v) end 
  local error_code = assert(get_error_code(hdrs))
  assert(error_code == 200, "good_state = " .. good_state)

-- SET STATE of T2 to started
  T.NewState = "started"
  hdrs = {} ; body = "" 
  addnl_curl_params.postfields = JSON:encode(T)
  local c = cURL.easy(addnl_curl_params)
  local x = c:perform()
  -- for k, v in pairs(hdrs) do print(k, v) end 
  local error_code = assert(get_error_code(hdrs))
  assert(error_code == 200, "good_state = " .. good_state)

-- SET STATE of T2 to terminated
  T.NewState = "terminated"
  T.Winner = "Control"
  hdrs = {} ; body = "" 
  addnl_curl_params.postfields = JSON:encode(T)
  local c = cURL.easy(addnl_curl_params)
  local x = c:perform()
  -- for k, v in pairs(hdrs) do print(k, v) end 
  local error_code = assert(get_error_code(hdrs))
  assert(error_code == 200, "good_state = " .. good_state)

-- SET STATE of T2 to archived
  T.NewState = "archived"
  hdrs = {} ; body = "" 
  addnl_curl_params.postfields = JSON:encode(T)
  local c = cURL.easy(addnl_curl_params)
  local x = c:perform()
  -- for k, v in pairs(hdrs) do print(k, v) end 
  local error_code = assert(get_error_code(hdrs))
  assert(error_code == 200, "good_state = " .. good_state)

-- T1 to follow T2


-- T1 changes channel to "aol"
  T.name = "T1"
  T.Winner = ""
  T.state_id = "1"
  T.state = "draft"
  T.Channel = "facebook"
  T.id = test_id_1
  curl_params.postfields = JSON:encode(T)
  local c = cURL.easy(curl_params)
  x = c:perform()
  local error_code = assert(get_error_code(hdrs))
  assert(error_code == 200)
  return T
-- Predeccessor id of T1 to be null




end



--===================================================
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
