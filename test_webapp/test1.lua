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
local tests = {}
tests.t1 = function (
  just_pr
  )
  if ( just_pr ) then 
    print(description)
    return
  end
  description = "A basic test "
  url = "localhost:8080/AB/php/endpoints/endpoint_test_basic.php"

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
  c = cURL.easy{
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
  return true
end
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
