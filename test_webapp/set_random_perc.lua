require 'lua/str'
local curl      = require 'lua/curl'
local random_perc      = require 'test_webapp/random_perc'
local tbp_url = "localhost:8080/AB/php/endpoints/endpoint_test_basic.php"

local function set_random_perc(
  tid
  )
  T = assert(random_perc(get_test_info(tid)))
  local a, b c = get_url(tbp_url)
  return T, c, a, b
end
return set_random_perc
