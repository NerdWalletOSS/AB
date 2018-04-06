
description = "A basic test "

url = "localhost:8080/AB/app/insert_test_processor.php"

JSON = (loadfile "JSON.lua")() -- one-time load of the routines

local cURL = require "cURL"
local ltn12 = require("ltn12")

function trim1(s)
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

local function test1(
  just_pr
  )
  if ( just_pr ) then 
    print(description)
    return
  end

  T = {}
  T.name = "T1"
  T.TestType = "XYTest"
  T.Creator = "joe"
  T.id = ""
  --==============
  local v = {}
  local Variants = {}
  v.name = "Control" v.percentage = "50" v.url = "www.google.com"
  Variants[#Variants+1] = v
  v.name = "V1" v.percentage = "30" v.url = "www.yahoo.com"
  Variants[#Variants+1] = v
  v.name = "V2" v.percentage = "20" v.url = "www.cnn.com"
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
  
  print("Performing curl")
  x = c:perform()
  print("Printing headers")
  for k, v in ipairs(hdrs) do print(k, v) end
  print("Printing body")
  print(body)
  print("ALL DONE")
end
test1()
