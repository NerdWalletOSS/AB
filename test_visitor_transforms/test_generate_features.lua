package.path = package.path .. ';../src/?.lua'
local generate_features = require 'generate_features'
local cache = require 'cache'
local json = require 'json'
local test_data = 'generate_features_data.json'
--_G.g_table_isn, _G.g_table_mvc, _G.g_table_rd_sm, _G.g_table_rd_search = assert(dofile('../config/referrer_class_file.lua'))
--_G.g_postal_cd_features = assert(dofile('../config/postal_cd_features.lua'))
local status, g_postal_cd_features = pcall(dofile, '../config/postal_cd_features.lua')
assert(status, "loading postal_cd_features.lua failed")
assert(g_postal_cd_features, "loading g_postal_cd_features failed")
cache.put("g_postal_cd_features", g_postal_cd_features)
--[[
local status, g_table_isn, g_table_mvc, g_table_rd_sm, g_table_rd_search = pcall(dofile, '../config/referer_class_file.lua')
assert(status, "loading referrer_class_file.lua failed")
assert(g_table_isn, "loading g_table_isn failed")
assert(g_table_mvc, "loading g_table_mvc failed")
assert(g_table_rd_sm, "loading g_table_rd_sm failed")
assert(g_table_rd_search, "loading g_table_rd_search failed")

cache.put("g_table_isn", g_table_isn)
cache.put("g_table_mvc", g_table_mvc)
cache.put("g_table_rd_sm", g_table_rd_sm)
cache.put("g_table_rd_search", g_table_rd_search)
]]--
local status, referer_class_tables = pcall(dofile, '../config/referer_class_file.lua')
assert(status, 'loading referrer_class_file failed')
assert(referer_class_tables, 'loading referer_class_tables failed')
assert(referer_class_tables["isn"], 'loading g_table_isn failed')
assert(referer_class_tables["mvc"], 'loading g_table_mvc failed')
assert(referer_class_tables["rd_sm"], 'loading g_table_rd_sm failed')
assert(referer_class_tables["rd_search"], 'loading g_table_rd_search failed')
cache.put("g_table_isn", referer_class_tables["isn"])
cache.put("g_table_mvc", referer_class_tables["mvc"])
cache.put("g_table_rd_sm", referer_class_tables["rd_sm"])
cache.put("g_table_rd_search", referer_class_tables["rd_search"])

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
