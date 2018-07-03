
-- TODO Fix hard coding the path
JSON = (loadfile "../test_webapp/JSON.lua")() -- one-time load of the routines
 T = {}
 T.uuid = "ABCD1234"
 T.test_id = "123"
 T.variant_id = "456";
 T.time = tostring(os.time())
 T.out_tracer_id = "ABCD_1234-678";

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

c = cURL.easy{
  url        = "localhost:8004/Log",
  post       = true,

  postfields = JSON:encode(T),
  -- postfields = '{"phello": "pworld"}';
  -- postfields = 'abc=123&def=456';
  httpheader = {
    "Content-Type: application/json";
    "X-Caller-Client-ID: ab-runtime-service";
  };
  writefunction  = snarf_body,
  headerfunction = snarf_headers,
}

x = c:perform()
print("Printing headers")
for k, v in ipairs(hdrs) do print(k, v) end
print("Printing body")
print(body)
print("ALL DONE")
