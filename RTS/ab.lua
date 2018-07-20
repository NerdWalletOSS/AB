-- ab.lua is the only place where we have globals rest all use locals.
-- All function call from C are registered here.
-- Also the globals are functions.
local x_load_config = require 'RTS/ab_load_config'
local x_update_config = require 'RTS/ab_update_config'

function load_config(...)
  x_load_config.load_config(...)
end

function update_config(...)
  x_update_config(...)
end
