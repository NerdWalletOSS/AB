package.path = package.path .. ';../src/?.lua'
local generate_features = require 'generate_features'
local cache = require 'cache'
local json = require 'json'
local test_data = 'generate_features_data.json'
local status, postal_cd_features = pcall(dofile, '../config/postal_cd_features.lua')
assert(status, "loading postal_cd_features.lua failed")
assert(postal_cd_features, "loading postal_cd_features failed")
cache.put("postal_cd_features", postal_cd_features)
local status, referer_class_tables = pcall(dofile, '../config/referer_class_file.lua')
assert(status, 'loading referrer_class_file failed')
assert(referer_class_tables, 'loading referer_class_tables failed')
assert(referer_class_tables["isn"], 'loading table_isn failed')
assert(referer_class_tables["mvc"], 'loading table_mvc failed')
assert(referer_class_tables["rd_sm"], 'loading table_rd_sm failed')
assert(referer_class_tables["rd_search"], 'loading table_rd_search failed')
cache.put("table_isn", referer_class_tables["isn"])
cache.put("table_mvc", referer_class_tables["mvc"])
cache.put("table_rd_sm", referer_class_tables["rd_sm"])
cache.put("table_rd_search", referer_class_tables["rd_search"])

describe("Testing generate_features", function()

    it("should be able to return the correct generate_features for 1000 entries.", function()
        local lines = {}
        for line in io.lines(test_data) do 
          lines[#lines + 1] = line
        end
        local json_dict = json.decode(lines[1])

        for _, pair in pairs(json_dict) do
            local lua_outputs = generate_features(pair['inputs'])
            local expected_outputs = pair['outputs']
            expected_outputs['mean_tu_cs'] = math.floor(expected_outputs['mean_tu_cs'] + 0.5)
            lua_outputs['mean_tu_cs'] = math.floor(lua_outputs['mean_tu_cs'] + 0.5)
            assert.are.same(expected_outputs, lua_outputs)
        end
    end)
end)
