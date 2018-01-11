
JSON = (loadfile "JSON.lua")() -- one-time load of the routines
 T = {}
 T.abc = { hello  = "world" }
 T.def = { hello2  = "world2" }

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
  url        = "173.255.252.157:8080/_foo.php?gkey=gval",
  post       = true,

  postfields = JSON:encode(T),
  -- postfields = '{"phello": "pworld"}';
  -- postfields = 'abc=123&def=456';
  httpheader = {
    "Content-Type: application/json";
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
