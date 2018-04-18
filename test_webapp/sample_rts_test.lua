package.path = package.path .. ";./../src/?.lua;./../test_webapp/?.lua;../test_generator/?.lua"
require 'str'
local json = require 'json'
local plfile = require 'pl.file'
local plpath = require 'pl.path'
local curl = require 'curl'
--===========================================
local url = "localhost:8000/Ignore"
local hdrs, body, status = curl.get(url)
assert(status == 200)
-- for k, v in pairs(hdrs) do print(k, v) end 
assert(body == '{ "Ignore" : "OK" }')
--==========================
url = "http://localhost:8000/AddTest"
hdrs = nil
local infile = "sample1.json"
assert(plpath.isfile(infile))
local inbody = assert(plfile.read(infile))
local hdrs, body, status = curl.post(url, nil, inbody)
print(status)
for k, v in pairs(hdrs) do print(k, v) end 
print(body)
--===========================================
