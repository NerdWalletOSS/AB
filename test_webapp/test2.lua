dofile('common.lua') -- TODO WHY NOT WORKING AS EXPECTED
local JSON = (loadfile "JSON.lua")() -- one-time load of the routines
local cURL = require "cURL"
local ltn12 = require("ltn12") -- TODO What is this for?
local plstr = require 'pl.stringx'
local plpath = require 'pl.path'
local get_test_id = require 'get_test_id'
local get_error_code = require 'get_error_code'
local function trim1(s)
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end
--============================
hdrs = {}
local function snarf_headers (str)
  hdrs[#hdrs+1] = trim1(str)
end
--============================
body = ""
local function snarf_body (str)
  body = str
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
  writefunction  = get_body,
  headerfunction = get_hdrs,
}
-- STOP: Stuff common to all tests in this suite
local tests = {}
tests.t1 = function (
  just_pr
  )
  description = "Channel Change of follower sets predecessor to null "
  if ( just_pr ) then print(description) return end

  os.execute("cd ../sql/; bash reset_db.sh; cd - ")
-- Create Test T1
  T = {}
  T.name = "T1"
  T.TestType = "XYTest"
  T.Creator = "joe"
  T.Channel = "facebook"
  T.id = ""
  --==============
  local v 
  local Variants = {}
  v = {} v.name = "Control" v.percentage = "50" v.url = "www.google.com"
  Variants[#Variants+1] = v
  v = {} v.name = "V1" v.percentage = "30" v.url = "www.yahoo.com"
  Variants[#Variants+1] = v
  v = {} v.name = "V2" v.percentage = "20" v.url = "www.cnn.com"
  Variants[#Variants+1] = v
  --==============
  T.Variants = Variants

  curl_params.postfields = JSON:encode(T)
  local c = cURL.easy(curl_params)
  x = c:perform()
  local test_id_1 = assert(get_test_id(hdrs))
  assert(test_id_1 > 0)
  local error_code = assert(get_error_code(hdrs))
  assert(error_code == 200)
  return T

-- Create Another Test T2
  hdrs = {} -- IMPORTANT: Needs to be reset because set in t1()
  body = "" -- IMPORTANT: Needs to be reset because set in t1()
  T.name = "T2"
  curl_params.postfields = JSON:encode(T)
  local c = cURL.easy(curl_params)
  x = c:perform()
  local test_id_2 = assert(get_test_id(hdrs))
  assert(test_id_2 > 0)
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
