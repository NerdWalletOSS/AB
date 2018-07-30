require 'lua/str'
local JSON = require 'lua/JSON'
local curl = require 'lua/curl'
--==========================
local function get_test_info(
  test_id
  )
  assert(type(test_id) == "number")
  
  local server = os.getenv("WEBAPP_SERVER") or "localhost"
  local url = "http://" .. server .. ":8080/AB/php/endpoints/endpoint_test_info.php?TestID=" .. test_id
  local hdrs, body, status = curl.post(url)
  assert(status == 200)
  local T = assert(JSON:decode(body))
  return T, hdrs
end
local dbg = false
if dbg then 
local T = get_test_info(1); 
for k, v in pairs(T.Variants) do 
  for k2, v2 in pairs(v) do 
    if ( ( k2 == "url" ) or ( k2 == "name" ) or ( k2 == "id" ) ) then 
      print(k, k2, v2) 
    end
  end
end 
end
return get_test_info
