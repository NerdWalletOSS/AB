package.path=package.path .. ";./../src/?.lua"
local assertx = require 'assertx'
-- local dbg = require 'debugger'
local ffi = require 'ab_ffi'
local json = require 'json'
local consts = require 'ab_consts'
local Tests = require 'tests'
local spooky_hash = require 'spooky_hash'
local g_tests = ffi.cast("TEST_META_TYPE*", ffi.C.malloc(ffi.sizeof("TEST_META_TYPE")*consts.AB_MAX_NUM_TESTS))
local c_index = ffi.cast("int*", ffi.C.malloc(ffi.sizeof("int")))
c_index[0] = -1
local valid_json = [[
{
  "TestType": "ABTest",
  "description": "some bogus description",
  "Creator": "joe",
  "name": "T1",
  "id": "777",
  "State": "started",
  "seed": "123453789",
  "Variants": [{
    "id": "100",
    "name": "Control",
    "percentage": "50",
    "url": "www.google.com"
  },
  {
    "id": "200",
    "name": "Variant_A",
    "percentage": "30",
    "url": "www.yahoo.com"
  },
  {
    "id": "300",
    "name": "Variant_B",
    "percentage": "20",
    "url": "www.cnn.com"
  }
  ]
}
]]

local function cleanup(g_tests, c_index)
  local index = tonumber(c_index[0])
  if index ~= -1 then
    ffi.fill(g_tests + index, ffi.sizeof("TEST_META_TYPE"))

  end

end

local function fill_g_tests()
  for i=1,consts.AB_MAX_NUM_TESTS do
    g_tests[i-1].name_hash=1007
  end
end

local function empty_g_tests()
  for i=1,consts.AB_MAX_NUM_TESTS do
    g_tests[i-1].name_hash=0
  end
end

describe('AddTests framework', function()
  before_each(function()

  end)

  after_each(function()

  end)
  describe(", for ABTests ", function()
    describe('should add a test which', function()
      it("should work for a valid json string", function()
        local status, res = pcall(Tests.add, valid_json, g_tests, c_index)
        assert(status == true, "Insert for valid test should succeed. Failure: ")
        assert(c_index[0] ~= -1, "Entry should have a valid index")
        cleanup(g_tests, c_index)
      end)

      it("should have a valid TestType", function()
        local j_table = json.decode(valid_json)
        j_table.TestType = nil
        local j_str = json.encode(j_table)
        local status, res = pcall(Tests.add, j_str, g_tests, c_index)
        assert(status == false)
        cleanup(g_tests, c_index)
      end)

      it("should have a valid name", function()
        local j_table = json.decode(valid_json)
        j_table.name = nil
        local j_str = json.encode(j_table)
        local status, res = pcall(Tests.add, j_str, g_tests, c_index)
        assert(status == false)
        cleanup(g_tests, c_index)

        j_table.name = string.rep("a", consts.AB_MAX_LEN_TEST_NAME + 1)
        j_str = json.encode(j_table)
        status, res = pcall(Tests.add, j_str, g_tests, c_index)
        assert(status == false)
        cleanup(g_tests, c_index)



        j_table.name = string.rep("a", consts.AB_MAX_LEN_TEST_NAME)
        j_str = json.encode(j_table)
        local status, res = pcall(Tests.add, j_str, g_tests, c_index)
        assert(status == true, "Insert for valid name should succeed")
        assert(c_index[0] ~= -1, "Entry should have a valid index")
        cleanup(g_tests, c_index)
      end)

      it("should have a valid position", function()
        -- se default position 1 before and 1 after are only positions left
        local j_table = json.decode(valid_json)
        local default_pos = spooky_hash.spooky_hash64(j_table.name, #j_table.name, Tests.g_seed1) % consts.AB_MAX_NUM_TESTS
        fill_g_tests()
        g_tests[default_pos].name_hash = 0
        local status, res = pcall(Tests.add, valid_json, g_tests, c_index)
        assertx(status == true, "Insert should succeed in default position ", res)
        assert(c_index[0] == default_pos, "should insert in empty position")
        assert(c_index[0] ~= -1, "Entry should have a valid index")
        cleanup(g_tests, c_index)

        fill_g_tests()
        g_tests[default_pos + 1].name_hash = 0
        local status, res = pcall(Tests.add, valid_json, g_tests, c_index)
        assert(status == true, "Insert should succeed in next to default position")
        assert(c_index[0] == default_pos + 1, "should insert in empty position")
        assert(c_index[0] ~= -1, "Entry should have a valid index")
        cleanup(g_tests, c_index)

        fill_g_tests()
        g_tests[default_pos - 1].name_hash = 0
        local status, res = pcall(Tests.add, valid_json, g_tests, c_index)
        assert(status == true, "Insert should succeed in previous to default  position")
        assert(c_index[0] == default_pos - 1, "should insert in empty position")
        assert(c_index[0] ~= -1, "Entry should have a valid index")
        cleanup(g_tests, c_index)
        empty_g_tests()
        
        
        fill_g_tests()
        local status, res = pcall(Tests.add, valid_json, g_tests, c_index)
        assert(status == false, "Insert should fail in a full tests array`")
        assert(c_index[0] == -1, "Entry should not exist")
        cleanup(g_tests, c_index)
        empty_g_tests()
      end)

      it("should have a valid TestType", function()
        local j_table = json.decode(valid_json)
        j_table.TestType = nil
        local j_str = json.encode(j_table)
        local status, res = pcall(Tests.add, j_str, g_tests, c_index)
        assert(status == false)
        cleanup(g_tests, c_index)
      end)
      it("should have a valid TestType", function()
        local j_table = json.decode(valid_json)
        j_table.TestType = nil
        local j_str = json.encode(j_table)
        local status, res = pcall(Tests.add, j_str, g_tests, c_index)
        assert(status == false)
        cleanup(g_tests, c_index)
      end)
    end)
  end)

  -- describe("for XYTests", function()
  -- end)

  describe("should fail for any other testtype", function()
    local j_table = json.decode(valid_json)
    j_table.TestType = "InvalidTest"
    local j_str = json.encode(j_table)
    local status, res = pcall(Tests.add, j_str, g_tests, c_index)
    assert(status == false)
    cleanup(g_tests, c_index)
  end)
end)
