describe("Testing get_referrer_type_cd", function()
    package.path = package.path .. ';../src/?.lua'
    local get_referrer_type_cd = require 'get_referrer_type_cd'
    local cache = require 'cache'
    local json = require 'json'
    local test_data = 'rtc_data.json'
    local table_isn, table_mvc, table_rd_sm, table_rd_search = dofile('../config/referrer_class_file.lua')
    cache.put("table_isn", table_isn)
    cache.put("table_mvc", table_mvc)
    cache.put("table_rd_sm", table_rd_sm)
    cache.put("table_rd_search", table_rd_search)

    it("should be able to return the correct referrer_type_cd for 1 million entries.", function()
        local lines = {}
        for line in io.lines(test_data) do 
          lines[#lines + 1] = line
        end
        local json_dict = json.decode(lines[1])

        for _, feature_table in pairs(json_dict) do
            local rtc = get_referrer_type_cd(feature_table)
            assert.are.same(rtc, feature_table['rtc_v2'])
        end
    end)
end)