package.path = package.path .. ';../src/?.lua'
local get_referrer_type_cd = require 'get_referrer_type_cd'
local cache = require 'cache'
local json = require 'json'
local test_data = 'rtc_data.json'
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


describe("Testing get_referrer_type_cd", function()

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
