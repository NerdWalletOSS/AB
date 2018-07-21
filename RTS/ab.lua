-- ab.lua is the only place where we have globals rest all use locals.
-- All function call from C are registered here.
-- Also the globals are functions.
local x_load_config   = require 'RTS/ab_load_config'
local x_update_config = require 'RTS/ab_update_config'

function load_config(...)
  x_load_config.load_config(...)
end

function update_config(...)
  x_update_config(...)
end

--TODO P3 Is this x_get_test hackery needed?
local x_get_test      = require 'RTS/get_test'
function get_test(...)
  x, y =  x_get_test(...)
  return x, y
end

local x_update_test      = require 'RTS/update_test'
function update_test(...)
  x =  x_update_test(...)
  return x
end

