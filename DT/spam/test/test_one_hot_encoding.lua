package.path = '../../../?.lua;../../?.lua;../?.lua;../lua/?.lua;../../lua/?.lua;../../../lua/?.lua;' .. package.path

local assertx = require 'assertx'
local cache = require 'cache'
local one_hot_encoding = require 'DT.lua.one_hot_encoding'
local json = require 'lua.json'
local test_data = 'ohe_data.json'
local dt_feature = require 'DT.cc_visitor_models.dt_feature'
assert(dt_feature, 'dt_feature not loaded')
cache.put("dt_feature", dt_feature)


describe("Testing one_hot_encoding", function()

    it("should be able to return the correct one_hot_encoding for test data.", function()
        local lines = {}
        for line in io.lines(test_data) do 
          lines[#lines + 1] = line
        end
        local json_dict = json.decode(lines[1])

        for i, pair in ipairs(json_dict) do
            local lua_ohe = one_hot_encoding(pair)
            for k, v in pairs(pair) do outputs[tonumber(k)] = tonumber(v) end
            assert.are.same(lua_ohe, outputs)
        end
    end)
end)
