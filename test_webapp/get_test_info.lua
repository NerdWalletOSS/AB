require 'lua/str'
local JSON = require 'lua/JSON'
local curl = require 'lua/curl'
--==========================
local function get_test_info(
  test_id
  )
  assert(type(test_id) == "number")
  local url = "http://localhost:8080/AB/php/endpoints/endpoint_test_info.php?TestID=" .. test_id
  local hdrs, body, status = curl.post(url)
  assert(status == 200)
  local T = assert(JSON:decode(body))
  return T, hdrs
end
return get_test_info
-- local T = get_test_info(1); for k, v in pairs(T) do print(k, v) end 
