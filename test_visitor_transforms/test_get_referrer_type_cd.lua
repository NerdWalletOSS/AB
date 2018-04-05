package.path = package.path .. ';../src/?.lua'
local get_referrer_type_cd = require 'get_referrer_type_cd'
local cache = require 'cache'
local json = require 'json'
local test_data = 'rtc_data.json'
--_G.g_table_isn, _G.g_table_mvc, _G.g_table_rd_sm, _G.g_table_rd_search = assert(dofile('../config/referrer_class_file.lua'))
local status, g_table_isn, g_table_mvc, g_table_rd_sm, g_table_rd_search = pcall(dofile, '../config/referrer_class_file.lua')
assert(status, 'loading referrer_class_file failed')
assert(g_table_isn, 'loading g_table_isn failed')
assert(g_table_mvc, 'loading g_table_mvc failed')
assert(g_table_rd_sm, 'loading g_table_rd_sm failed')
assert(g_table_rd_search, 'loading g_table_rd_search failed')
cache.put("g_table_isn", g_table_isn)
cache.put("g_table_mvc", g_table_mvc)
cache.put("g_table_rd_sm", g_table_rd_sm)
cache.put("g_table_rd_search", g_table_rd_search)
--
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