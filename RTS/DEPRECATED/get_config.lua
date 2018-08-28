local cache = require 'lua/cache'
local JSON  = require 'lua/JSON'

return function()
  return JSON:encode(cache.get('config'))
end
