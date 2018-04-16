package.path=package.path .. ";./../src/?.lua;./../test_webapp/?.lua;../test_generator/?.lua"
require 'str'
local json = require 'json'
local curl = require 'curl'
-- local dbg = require 'debugger'
local file = assert(io.open("../test_webapp/sample1.json"), "File should exist")
local test = json.decode(file:read("*a"))
file:close()
local url = "localhost:8000/addTest"
curl.post(url, nil, test)
local results = {}
for key,entry in pairs(test.DeviceCrossVariant) do
  local tbl = {}
  -- local device_id = entry[1].device_id
  local c_url = string.format("http://localhost:8000/Router?TestName=T1&Device=%s", key)
  for i=1,10000 do
    local hdrs, body, status = curl.get(c_url)
    assert(status == 302, "Must be redirecting")
    local redirected_url = hdrs[3]:split(" ")[2]:split("?")[1]
    tbl[redirected_url] = (tbl[redirected_url] or 0) + 1
  end
  results[key] = tbl
end
local totals = {}
for k,v in pairs(results) do
  local total = 0
  for _, v2 in pairs(v) do
    total = total + v2
  end
  totals[k] = total
end

for k,v in pairs(results) do
  for dev, count in pairs(v) do
    print(k, dev, count*100.0/totals[k], " total: ", count)
  end
end
