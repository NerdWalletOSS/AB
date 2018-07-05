local cache = require 'lua/cache'
local load_config = require 'RTS/ab_load_config'

function update_config()
  local config = assert(cache.get("config"))
  assert(config, "a configuration needs to be passed to ab_load_config before upsate is called")
  load_config.load_db_data(config.AB.MYSQL)
end
return update_config
