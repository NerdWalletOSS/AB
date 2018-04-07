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
-- STOP: Stuff common to all tests in this suite
local tests = {}
tests.t1 = function (
  just_pr
  )
  description = "A basic test "
  if ( just_pr ) then print(description) return end

  os.execute("cd ../sql/; bash reset_db.sh; cd - ")
  T = {}
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
  local c = cURL.easy(curl_params)
  x = c:perform()

  local test_id = assert(get_test_id(hdrs))
  assert(test_id > 0)
  local error_code = assert(get_error_code(hdrs))
  assert(error_code == 200)
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

  for i = 1, 100000 do 
    os.execute("cd ../sql/; bash reset_db.sh; cd - ")
    local filename = "basic" .. i .. ".lua"
    if ( not plpath.isfile(filename) )  then break end 
    T = dofile("good_basic" .. i .. ".lua")
    hdrs = {} 
    body = "" 
    curl_params.postfields = JSON:encode(T)
    local c = cURL.easy(curl_params)
    x = c:perform()
    local test_id = assert(get_test_id(hdrs))
    assert(test_id > 0)
    local error_code = assert(get_error_code(hdrs))
    assert(error_code == 200)
    print("Tested " .. filename)
  end
  print("Test t3 succeeded")
  return true
end
--===================================================
tests.t4 = function (
  just_pr
  )
  description = "Create a number of invalid tests "
  if ( just_pr ) then print(description) return end

  for i = 1, 100000 do 
    os.execute("cd ../sql/; bash reset_db.sh; cd - ")
    local filename = "basic" .. i .. ".lua"
    if ( not plpath.isfile(filename) )  then break end 
    T = dofile("bad_basic" .. i .. ".lua")
    hdrs = {} 
    body = "" 
    curl_params.postfields = JSON:encode(T)
    local c = cURL.easy(curl_params)
    x = c:perform()
    local error_code = assert(get_error_code(hdrs))
    assert(error_code == 400)
    print("Tested " .. filename)
  end
  print("Test t4 succeeded")
  return true
end
--===================================================
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
tests.t2() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
tests.t3() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
tests.t4() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
