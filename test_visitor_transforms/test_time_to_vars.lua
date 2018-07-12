package.path = package.path .. ';../src/?.lua'
local json = require 'json'
local time_to_vars = require 'time_to_vars'
local luatz = require 'luatz.init'
local math = require 'math'
local MAX_TS = 2147483648
local good_tz = {'America/New_York', 'America/Detroit', 'America/Kentucky/Louisville', 'America/Kentucky/Monticello', 'America/Indiana/Indianapolis', 'America/Indiana/Vincennes', 'America/Indiana/Winamac', 'America/Indiana/Marengo', 'America/Indiana/Petersburg', 'America/Indiana/Vevay', 'America/Chicago', 'America/Indiana/Tell_City', 'America/Indiana/Knox', 'America/Menominee', 'America/North_Dakota/Center', 'America/North_Dakota/New_Salem', 'America/North_Dakota/Beulah', 'America/Denver', 'America/Boise', 'America/Phoenix', 'America/Los_Angeles', 'America/Anchorage', 'America/Juneau', 'America/Sitka', 'America/Yakutat', 'America/Nome', 'America/Adak', 'Pacific/Honolulu'}
local inputs_and_outputs = 'time_data.json'

describe("Testing time to vars", function()

    it("should be able to give out valid outputs to 1000
        good randomly generated inputs (timestamp as numeric)", function()
        for i=1,1000 do
            t = math.random(MAX_TS)
            tz = good_tz[math.random(#good_tz)]
            assert.truthy(time_to_vars(t, tz))
        end
    end)

    it("should be able to give out valid outputs to 1000
        good randomly generated inputs (timestamp as string)", function()
        for i=1,1000 do
            t = tostring(math.random(MAX_TS))
            tz = good_tz[math.random(#good_tz)]
            assert.truthy(time_to_vars(t, tz))
        end
    end)

    it("should match 1000 outputs from the original Python
        script when given valid inputs", function()
        local lines = {}
        for line in io.lines(inputs_and_outputs) do 
            lines[#lines + 1] = line
        end
        local json_dict = json.decode(lines[1])
        for i, pair in ipairs(json_dict) do
            assert.are.same(time_to_vars(pair['inputs']['timestamp'],
                pair['inputs']['timezone']), pair['outputs'])
        end
    end)

    it("should be able to deal with rfc 3339 strings.", function()
        assert.are.same(time_to_vars("2018-01-01T23:03:00", 'America/New_York'), {
            ["day_of_wk"]=0, 
            ["year"]=2018,
            ["is_late_at_night"]=false,
            ["local_hour"]=18,
            ["utc_offset"]='-5.0',
            ["month"]=1,
            ["is_working"]=true})
    end)

    it("should fail when timestamp is too small", function()
        assert.is_false(pcall(time_to_vars, '-0.1', 'America/New_York'))
    end)

    it("should fail when timestamp is too large", function()
        assert.is_false(pcall(time_to_vars, '100000000000', 'America/New_York'))
    end)

    it("should fail when timestamp is invalid", function()
        assert.is_false(pcall(time_to_vars, '2012-01-333T03:78:00', 'America/New_York'))
    end)


    it("should fail when timestamp is invalid", function()
        assert.is_false(pcall(time_to_vars, '2012-01-33T03:78:00', 'America/NewYork'))
    end)
end)