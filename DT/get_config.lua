local JSON = require 'lua/JSON'
-- get config
local function get_config()
  return JSON:encode(conf)
end
return get_config
