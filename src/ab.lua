local csv = require "csv"
local json = require "json"
-- local ffi = require 'ffi' 
local tests = require 'add_test'
local load_config = require 'load_config'
local cache = require 'cache'
local reload_tests = require 'reload'

function add(...)
  tests.add(...)
end

function load_config(...)
  load_config.load_config_from_file(...)
end

function list_tests()
  return json.encode(cache.get('tests'))
end

function get_config()
  return cache.get('config')
end

function reload(...)
  return reload_tests.reload(...)
end
-- function add(c_str, c_data)
--    -- print("hi from lua", c_str)
--    local x = ffi.cast("int*", c_data)
--    print(c_str)
--    x[0] = 9
-- end
