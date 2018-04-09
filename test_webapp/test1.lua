-- TODO Move all common stuff into one file 
require 'strict'
local JSON = (loadfile "JSON.lua")() -- one-time load of the routines
local cURL = require "cURL"
local ltn12 = require("ltn12") -- TODO What is this for?
local plstr = require 'pl.stringx'
local plpath = require 'pl.path'
local get_test_id = require 'get_test_id'
local get_error_code = require 'get_error_code'
local x = require('get_hdrs')
local get_hdrs    = x[1]
local reset_hdrs  = x[2]
local return_hdrs = x[3]
local x = require('get_body')
local get_body    = x[1]
local reset_body  = x[2]
local return_body = x[3]
local get_url = require 'get_url'
local test_compare = require 'test_compare'
--============================
-- START: Stuff common to all tests in this suite
curl_params = { 
  url = "localhost:8080/AB/php/endpoints/endpoint_test_basic.php",
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

  os.execute("cd ../sql/; bash reset_db.sh; cd - ")
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
  x = c:perform()
  local body = return_body(); local hdrs = return_hdrs()
  print("XXXXXX")
  for k,v in pairs(hdrs) do print(k, v) end 
  print("XXXXXX")

  local test_id = assert(get_test_id(hdrs))
  assert(test_id > 0)
  local error_code = assert(get_error_code(hdrs))
  assert(error_code == 200)
  -- Check that test info is same as what you sent in
  local chk_url = "localhost:8080/AB/php/endpoints/endpoint_test_info.php?TestID=" .. test_id
  local body, hdrs_out = get_url(chk_url)
  local Tout = assert(JSON:decode(body), chk_url)
  assert(test_compare(T, Tout))
  print("Test t1 succeeded")
  return T
end
tests.t2 = function (
  just_pr
  )
  description = "Cannot create 2 tests with same name"
  if ( just_pr ) then print(description) return end

  os.execute("cd ../sql/; bash reset_db.sh; cd - ")
  local T = tests.t1()
  hdrs = {} -- IMPORTANT: Needs to be reset because set in t1()
  body = "" -- IMPORTANT: Needs to be reset because set in t1()

  curl_params.postfields = JSON:encode(T)
  local c = cURL.easy(curl_params)
  x = c:perform()

  local error_code = get_error_code(hdrs)
  assert(error_code == 400)
  print("Test t2 succeeded")
  return true
end
--===================================================
tests.t3 = function (
  just_pr
  )
  description = "Create a number of valid tests "
  if ( just_pr ) then print(description) return end

  local ngood = 0
  for i = 1, 100000 do 
    os.execute("cd ../sql/; bash reset_db.sh; cd - ")
    local filename = "good_basic" .. i .. ".lua"
    if ( not plpath.isfile(filename) )  then break end 
    T = dofile(filename)
    for k, v in pairs(hdrs) do print(k, v) end 
    hdrs = {} 
    body = "" 
    curl_params.postfields = JSON:encode(T)
    local c = cURL.easy(curl_params)
    x = c:perform()
    for k, v in pairs(hdrs) do print(k, v) end 
    local test_id = assert(get_test_id(hdrs))
    assert(test_id > 0)
    local error_code = assert(get_error_code(hdrs))
    assert(error_code == 200)
    ngood = ngood + 1
  end
  print("Test t3 [" .. ngood .. "] succeeded")
  return true
end
--===================================================
tests.t4 = function (
  just_pr
  )
  description = "Create a number of invalid tests "
  if ( just_pr ) then print(description) return end

  local nbad = 0
  for i = 1, 100000 do 
    os.execute("cd ../sql/; bash reset_db.sh; cd - ")
    local filename = "bad_basic" .. i .. ".lua"
    if ( not plpath.isfile(filename) )  then break end 
    T = dofile(filename)
    hdrs = {} 
    body = "" 
    curl_params.postfields = JSON:encode(T)
    local c = cURL.easy(curl_params)
    x = c:perform()
    for k, v in pairs(hdrs) do print(k, v) end 
    local error_code = assert(get_error_code(hdrs))
    assert(error_code == 400, "Failure on " .. filename)
    nbad = nbad + 1
    print("t4 Succeeded on " .. filename)
  end
  print("Test t4 [" .. nbad .. "] succeeded")
  return true
end
--===================================================
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
os.execute("sleep 1")
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
-- tests.t2() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
-- tests.t4() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
-- tests.t3() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
