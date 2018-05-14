local cache = require 'lua.cache'
local assertx = require 'lua.assertx'
local dt_feature = require 'DT.spam.dt_feature'
assert(dt_feature, 'loading dt_feature failed')
cache.put("dt_feature", dt_feature)

describe("Testing get_num_features", function()

    local get_num_features = require 'DT.lua.get_num_features'
    it("should get you correct numbers under visitor model (hardcoded)", function()
        assert.are.same(get_num_features(), 57)
    end)

    it("should get you correct numbers for non-nested dicts", function()
        cache.put("dt_feature", {[0]=4, [4]=9})
        assert.are.same(get_num_features(), 2)
    end)

    it("should get you correct numbers for nested dicts", function()
        cache.put("dt_feature", {[1]={[2]={[3]=4, [4]=5}, [5]=6}, [6]=1})
        assert.are.same(get_num_features(), 4)
    end)

    it("should get you correct numbers when nothing exists", function()
        cache.put("dt_feature", {})
        assert.are.same(get_num_features(), 0)
        cache.put("dt_feature", dt_feature)
    end)
end)
