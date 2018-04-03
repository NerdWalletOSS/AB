package.path = package.path .. ';../src/?.lua'
local one_hot_encoding = require 'one_hot_encoding'
local cache = require 'cache'
local json = require 'json'
local test_data = 'ohe_data.json'
local table_dt_feature = dofile('../config/dt_feature.lua')
cache.put("table_dt_feature", table_dt_feature)


describe("Testing one_hot_encoding", function()

    it("should be able to return the correct one_hot_encoding for test data.", function()
        local lines = {}
        for line in io.lines(test_data) do 
          lines[#lines + 1] = line
        end
        local json_dict = json.decode(lines[1])

        for i, pair in ipairs(json_dict) do
            local lua_ohe = one_hot_encoding(pair['inputs'])
            local outputs = {}
            for k, v in pairs(pair['outputs']) do outputs[tonumber(k)] = tonumber(v) end
            assert.are.same(lua_ohe, outputs)
        end
    end)
end)