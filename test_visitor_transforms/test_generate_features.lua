package.path = package.path .. ';../src/?.lua'
local generate_features = require 'generate_features'
local cache = require 'cache'
local json = require 'json'
local test_data = 'generate_features_data.json'
--_G.g_table_isn, _G.g_table_mvc, _G.g_table_rd_sm, _G.g_table_rd_search = assert(dofile('../config/referrer_class_file.lua'))
--_G.g_postal_cd_features = assert(dofile('../config/postal_cd_features.lua'))
local status, g_postal_cd_features = pcall(dofile, '/opt/ab/postal_cd_features.lua')
assert(status, "loading postal_cd_features.lua failed")
assert(g_postal_cd_features, "loading g_postal_cd_features failed")
local status, g_table_isn, g_table_mvc, g_table_rd_sm, g_table_rd_search = pcall(dofile, '/opt/ab/referer_class_file.lua')
assert(status, "loading referrer_class_file.lua failed")
assert(g_table_isn, "loading g_table_isn failed")
assert(g_table_mvc, "loading g_table_mvc failed")
assert(g_table_rd_sm, "loading g_table_rd_sm failed")
assert(g_table_rd_search, "loading g_table_rd_search failed")

cache.put("g_postal_cd_features", g_postal_cd_features)
cache.put("g_table_isn", g_table_isn)
cache.put("g_table_mvc", g_table_mvc)
cache.put("g_table_rd_sm", g_table_rd_sm)
cache.put("g_table_rd_search", g_table_rd_search)


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
