describe("Testing generate_features", function()
    package.path = package.path .. ';../src/?.lua'
    local generate_features = require 'generate_features'
    local cache = require 'cache'
    local json = require 'json'
    local test_data = 'generate_features_data.json'
    local table_isn, table_mvc, table_rd_sm, table_rd_search = dofile('../config/referrer_class_file.lua')
    local table_postal_cd_features = dofile('../config/postal_cd_features.lua')
    cache.put("table_postal_cd_features", table_postal_cd_features)
    cache.put("table_isn", table_isn)
    cache.put("table_mvc", table_mvc)
    cache.put("table_rd_sm", table_rd_sm)
    cache.put("table_rd_search", table_rd_search)

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