require 'lua/str'
local JSON = require 'lua/JSON'
local curl = require 'lua/curl'
--==========================
local function set_filters(
  T
  )
  assert(type(T) == "table")
  local url = "http://localhost:8080/AB/php/endpoints/endpoint_set_filters.php"
  local plfile = require 'pl.file'
  plfile.write("/tmp/_x.json", JSON:encode(T))
  local hdrs, body, status = curl.post(url, nil, JSON:encode(T))
  assert(status == 200)
  return hdrs
end
return set_filters
