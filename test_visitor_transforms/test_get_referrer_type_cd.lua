describe("Testing get_referrer_type_cd", function()
    package.path = package.path .. ';../src/?.lua'
    local get_referrer_type_cd = require 'get_referrer_type_cd'
    local cache = require 'cache'
    local json = require 'json'
    local test_data = 'rtc_data.json'
    local isn, mvc, rd_sm, rd_search = dofile('../config/referrer_class_file.lua')
    cache.put("isn", isn)
    cache.put("mvc", mvc)
    cache.put("rd_sm", rd_sm)
    cache.put("rd_search", rd_search)

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