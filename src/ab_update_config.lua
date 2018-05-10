local cache = require 'cache'

local function update_config()
  local config = assert(cache.get("config"))
  assert(config.AB)
  local db = config.AB.DB
  if ( db ) then 
    -- you need to make sure that ypu have valid stuff
    -- connect to database and verify 
  end
end
return update_config
