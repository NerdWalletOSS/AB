local csv = require "csv"
local json = require("json")
-- local ffi = require 'ffi' 
local tests = require 'tests'
function add(...)
  tests.add(...)
end

-- function add(c_str, c_data)
--    -- print("hi from lua", c_str)
--    local x = ffi.cast("int*", c_data)
--    print(c_str)
--    x[0] = 9
-- end
