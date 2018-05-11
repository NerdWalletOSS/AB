local cache = require 'lua/cache'
local load_config = require 'RTS/ab_load_config'

local function update_config()
  local config = assert(cache.get("config"))
  assert(config.AB)
  local db = config.AB.DB
  if ( db ) then 
    local conn = load_config.db_connect(db)
    assert(conn ~= nil, "must be a valid connection object")
  end
end
return update_config
