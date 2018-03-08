local csv = require "csv"
local json = require "json"
-- local ffi = require 'ffi' 
local tests = require 'tests'
local load_config = require 'load_config'
local cache = require 'cache'
function add(...)
  tests.add(...)
end

function load_config(...)
  load_config.load_config_from_file(...)
end

function list_tests()
  return json.encode(cache.get('tests'))
end
-- function add(c_str, c_data)
--    -- print("hi from lua", c_str)
--    local x = ffi.cast("int*", c_data)
--    print(c_str)
--    x[0] = 9
-- end
