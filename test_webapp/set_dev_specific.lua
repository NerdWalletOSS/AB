require 'lua/str'
local JSON    = require 'lua/JSON'
local curl    = require 'lua/curl'
local get_test_id = require 'test_webapp/get_test_id'
local get_test_info = require 'test_webapp/get_test_info'

local server = os.getenv("WEBAPP_SERVER") or "localhost"
local dsp_url = "http://" .. server .. ":8080/AB/php/endpoints/endpoint_set_dev_specific.php"

local function set_dev_specific(
  X
  )
  local hdrs, outbody, status = curl.post(dsp_url, nil, JSON:encode(X))
  -- print(outbody); for k, v in pairs(hdrs) do print(k, v) end
  return hdrs, outbody, status
end
return set_dev_specific
