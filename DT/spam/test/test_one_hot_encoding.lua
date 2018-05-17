local plfile           = require 'pl.file'
local plpath           = require 'pl.path'
local assertx          = require 'lua/assertx'
local cache            = require 'lua/cache'
local one_hot_encoding = require 'DT/lua/one_hot_encoding'
local JSON             = require 'lua/JSON'
local dt_feature       = require 'DT/spam/dt_feature'
assert(dt_feature, 'dt_feature not loaded')
cache.put("dt_feature", dt_feature)

describe("Testing one_hot_encoding", function()

    it("should be able to return the correct one_hot_encoding for test data.", function()
   local test_data = 'ohe_data.json'
   assert(plpath.isfile(test_data))
   local json_str = assert(plfile.read(test_data))
   print("loading JSON")
   local json_dict = assert(JSON:decode(json_str))
   assert(type(json_dict) == "table")

   for i, pair in ipairs(json_dict) do
     local lua_ohe = one_hot_encoding(pair)
     for k, v in pairs(pair) do outputs[tonumber(k)] = tonumber(v) end
       assert.are.same(lua_ohe, outputs)
     end
  end)
end)
