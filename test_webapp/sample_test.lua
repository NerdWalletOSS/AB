package.path = package.path .. ";./../src/?.lua;./../test_webapp/?.lua;../test_generator/?.lua"
require 'str'
local json = require 'json'
local curl = require 'curl'
local url = "localhost:8000/Ignore"
local hdrs, body, status = curl.get(url)
assert(status == 200)
for k, v in pairs(hdrs) do print(k, v) end 
print(body)
--==========================
url = "http://localhost:8000/AddTest"
hdrs = nil
local file = assert(io.open("sample1.json"), "File not found")
local inbody = json.decode(file:read("*a"))
file:close()
local hdrs, body, status = curl.post(url, nil, inbody)
print(status)
for k, v in pairs(hdrs) do print(k, v) end 
print(body)
--===========================================
