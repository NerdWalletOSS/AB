local cache = require 'cache'
local x_load_config = require 'dt_load_config'
local x_hard_code_config = require 'hard_code_config'
local x_update_config = require 'dt_update_config'

function load_config(...)
  cache.put("config", x_load_config(...))
end

function hard_code_config(...)
  cache.put("config", x_hard_code_config(...))
end

function update_config(...)
  x_update_config(...)
end

