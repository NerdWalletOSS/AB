-- ab.lua is the only place where we have globals rest all use locals.
-- All function call from C are registered here.
-- Also the globals are functions.
aux = {} -- TODO P3 rename to g_aux 

local x_load_config   = require 'RTS/ab_load_config'
function load_config(...)
  x_load_config.load_config(...)
end


local x_update_config = require 'RTS/ab_update_config'
function update_config(...)
  x_update_config(...)
end

--
--TODO P3 Is this x_function hackery needed?
local x_get_test      = require 'RTS/get_test'
assert(type(x_get_test) == "function")
function get_test(...)
  x, y =  x_get_test(...)
  return x, y
end

local x_update_test      = require 'RTS/update_test'
assert(type(x_update_test) == "function")
function update_test(...)
  x =  x_update_test(...)
  return x
end

local x_chk_device      = require 'RTS/chk_device'
assert(type(x_chk_device) == "function")
function chk_device(...)
  x =  x_chk_device(...)
  return x
end

local x_load_aux      = require 'RTS/load_aux'
assert(type(x_load_aux) == "function")
function load_aux(...)
  x, y =  x_load_aux(...)
  return x, y
end

return "ab loaded"
