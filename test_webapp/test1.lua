dofile('common.lua') -- TODO WHY NOT WORKING AS EXPECTED
local JSON = (loadfile "JSON.lua")() -- one-time load of the routines
local cURL = require "cURL"
local ltn12 = require("ltn12") -- TODO What is this for?
local plstr = require 'pl.stringx'
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
  local c = cURL.easy{
    url        = url,
    post       = true,
    postfields = JSON:encode(T),
    httpheader = { "Content-Type: application/json"; };
    writefunction  = snarf_body,
    headerfunction = snarf_headers,
  }
  
  x = c:perform()
  local test_id 
  for k, v in ipairs(hdrs) do 
    if ( string.find(v, "TestID") == 1 ) then 
      test_id = assert(string.gsub(v, "TestID: ", ""))
      test_id = assert(tonumber(test_id))
    end
  end
  assert(test_id > 0)
  print("Test t1 succeeded")
  return T, c
end
tests.t2 = function (
  just_pr
  )
  description = "Cannot create 2 tests with same name"
  if ( just_pr ) then print(description) return end

  os.execute("cd ../sql/; bash reset_db.sh; cd - ")
  local T, c = tests.t1()
  hdrs = {} -- IMPORTANT: Needs to be reset because set in t1()
  body = "" -- IMPORTANT: Needs to be reset because set in t1()
  local x = c:perform()
  local error_code
  for k, v in ipairs(hdrs) do 
    local needle = 'Error%-Code:' -- REMEMBER TO ESCAPE THE HYPHEN
    if ( string.find(v, needle) == 1 ) then 
      error_code = assert(string.gsub(v, needle, ""))
      error_code = assert(tonumber(error_code))
    end
  end
  assert(error_code == 400)
  print("Test t2 succeeded")
  return true
end
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
tests.t2() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
