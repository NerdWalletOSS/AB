package.path = package.path .. ";./../src/?.lua;./../test_webapp/?.lua;../test_generator/?.lua"
require 'str'
local json = require 'json'
local plfile = require 'pl.file'
local plpath = require 'pl.path'
local curl = require 'curl'
--==========================
local url = "http://localhost:8080/AB/index.html"
local hdrs, body, status = curl.get(url)
assert(status == 200)
-- for k, v in pairs(hdrs) do print(k, v) end 
-- print(body)
--==========================
url = "http://localhost:8080/AB/php/endpoints/endpoint_test_basic.php";
hdrs = nil
local infile = "good_test_basic_1.json"
assert(plpath.isfile(infile))
local inbody = assert(plfile.read(infile))
local hdrs, body, status = curl.post(url, nil, inbody)
assert(status == 200)
for k, v in pairs(hdrs) do print(k, v) end 
print(body)
print("\nALL DONE\n")
--===========================================
