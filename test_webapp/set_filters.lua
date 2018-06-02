require 'lua/str'
local JSON = require 'lua/JSON'
local curl = require 'lua/curl'
--==========================
local function set_filters(
  T
  )
  assert(type(T) == "table")
  local url = "http://localhost:8080/AB/php/endpoints/endpoint_set_filters.php"
  local hdrs, body, status = curl.post(url, nil, JSON:encode(T))
  for k,v in pairs(hdrs) do print(k, v) end
  print(body)
  print(status)
  assert(status == 200)
  return hdrs
end
return set_filters
