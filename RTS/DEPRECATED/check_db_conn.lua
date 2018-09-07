local cache = require 'lua/cache'
local load_config = require 'RTS/ab_load_config'

return function()
  local configs = cache.get('config').AB.DB
  local conn = load_config.db_connect(configs)
  assert(conn ~= nil, "must be a valid connection object")
  conn:close() -- TODO RS Integrate into webserver
end
