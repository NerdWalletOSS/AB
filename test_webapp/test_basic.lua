-- TODO Move all common stuff into one file 
require 'strict'
local JSON = (loadfile "JSON.lua")() -- one-time load of the routines
local cURL = require "cURL"
local ltn12 = require("ltn12") -- TODO What is this for?
local plstr = require 'pl.stringx'
local plpath = require 'pl.path'
local get_test_id = require 'get_test_id'
local get_error_code = require 'get_error_code'
local test_compare = require 'test_compare'
local get_url = require 'get_url'
local reset_db = require 'reset_db'
-- local make_rand_test = require 'XXXXX' TODO 

local suite_description = [[
  These tests verfy that basic addition and modification of a test
  as permissible on Screen 1 can be performed. 
]]

local hdrs = {}
local function trim1(s)
  return (s:gsub("^%s*(.-)%s*$", "%1"))
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
local body = ""
local function get_body (str)
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
  url = url,
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
  v = {} v.name = "Control" v.percentage = "50" v.url = "www.google.com"
  Variants[#Variants+1] = v
  v = {} v.name = "V1" v.percentage = "30" v.url = "www.yahoo.com"
  Variants[#Variants+1] = v
  v = {} v.name = "V2" v.percentage = "20" v.url = "www.cnn.com"
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
  -- Check that test info is same as what you sent in
  local chk_url = "localhost:8080/AB/php/endpoints/endpoint_test_info.php?TestID=" .. test_id

  local body_out, hdrs_out = get_url(chk_url)
  local Tout = assert(JSON:decode(body_out), chk_url)
  assert(test_compare(T, Tout))

  print("Test t1 succeeded")
  return T
end
--===================================================
tests.t2 = function (
  just_pr
  )
  local description = "Cannot create 2 tests with same name"
  if ( just_pr ) then print(description) return end

  reset_db()
  local T = tests.t1()
  hdrs = {} -- IMPORTANT: Needs to be reset because set in t1()
  body = "" -- IMPORTANT: Needs to be reset because set in t1()

  curl_params.postfields = JSON:encode(T)
  local c = cURL.easy(curl_params)
  local x = c:perform()

  local error_code = get_error_code(hdrs)
  assert(error_code == 400)
  print("Test t2 succeeded")
  return true
end
--===================================================
tests.t3 = function (
  just_pr
  )
  local description = "Create a number of valid tests "
  if ( just_pr ) then print(description) return end

  local ngood = 0
  local idx = 1
  while true do 
    reset_db()
    local filename = "good_basic" .. idx .. ".lua"
    if ( not plpath.isfile(filename) )  then break end 
    local T = dofile(filename)
    hdrs = {} ; body = "" 
    curl_params.postfields = JSON:encode(T)
    local c = cURL.easy(curl_params)
    local x = c:perform()
    local test_id = assert(get_test_id(hdrs))
    assert(test_id > 0)
    local error_code = assert(get_error_code(hdrs))
    assert(error_code == 200)
    ngood = ngood + 1
    print("t3 Succeeded on " .. filename)
    idx = idx + 1
  end
  print("Test t3 [" .. ngood .. "] succeeded")
  return true
end
--===================================================
tests.t4 = function (
  just_pr
  )
  local description = "Create a number of invalid tests "
  if ( just_pr ) then print(description) return end

  local nbad = 0
  local idx = 1
  while true do 
    reset_db()
    local filename = "bad_basic" .. idx .. ".lua"
    if ( not plpath.isfile(filename) )  then break end 
    local T = dofile(filename)
    hdrs = {} ; body = "" 
    curl_params.postfields = JSON:encode(T)
    local c = cURL.easy(curl_params)
    local x = c:perform()
    local error_code = assert(get_error_code(hdrs))
    assert(error_code == 400, "Failure on " .. filename)
    nbad = nbad + 1
    print("t4 Succeeded on " .. filename)
    idx = idx + 1
  end
  print("Test t4 [" .. nbad .. "] succeeded")
  return true
end
tests.t5 = function (
  just_pr
  )
  local description = "Make a random test "
  if ( just_pr ) then print(description) return end

  reset_db()
  local T = make_rand_test()  -- TODO 
  local status, body, hdrs = do_curl(url, T, nil, "POST")
  local test_id = assert(get_test_id(hdrs))
  assert(test_id > 0)
  local error_code = assert(get_error_code(hdrs))
  assert(error_code == 200)
  -- Check that test info is same as what you sent in
  local chk_url = "localhost:8080/AB/php/endpoints/endpoint_test_info.php?TestID=" .. test_id

  local body_out, hdrs_out = get_url(chk_url)
  local Tout = assert(JSON:decode(body_out), chk_url)
  assert(test_compare(T, Tout))
  print("Test t5 succeeded")
  return T
end
--===================================================
--===================================================
--===================================================
tests.t1() 
tests.t2() 
tests.t3() 
tests.t4() 
return tests, suite_description
