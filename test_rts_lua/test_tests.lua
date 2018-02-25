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
local valid_json = require 'valid_json'
local valid_json2 = require 'valid_json2'

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
        assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
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
        assertx(status == true, "Insert for valid name should succeed Failure: ", res)
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

      describe("for c_to_v_ok_v_to_c_ok_v_to_v_not_ok", function()
        it("should have a control type", function()
          local j_table = json.decode(valid_json)
          j_table.Variants[1].name = 'failme'
          local j_str = json.encode(j_table)
          local status, res = pcall(Tests.add, j_str, g_tests, c_index)
          assert(status == false)
          cleanup(g_tests, c_index)
        end)
        it('reordered variants should result in same ordering', function()
          local j_table = json.decode(valid_json)
          table.sort(j_table.Variants, function(a,b) return a.name > b.name end)
          local j_str = json.encode(j_table)
          local status, res = pcall(Tests.add, j_str, g_tests, c_index)
          assert(status == true, 'Insertion for valid json should succeed')
          local variant_bin = g_tests[c_index[0]].variant_per_bin[0]
          local bin_size = ffi.sizeof(string.format("uint8_t[%s]", consts.AB_NUM_BINS))
          local old_bin = ffi.cast("uint8_t*", ffi.gc(ffi.C.malloc(bin_size), ffi.C.free))
          ffi.copy(old_bin , variant_bin, bin_size)
          cleanup(g_tests, c_index)

          table.sort(j_table.Variants, function(a,b) return a.name < b.name end)
          j_str = json.encode(j_table)
          status, res = pcall(Tests.add, j_str, g_tests, c_index)
          variant_bin = g_tests[c_index[0]].variant_per_bin[0]
          for i=1,consts.AB_NUM_BINS do
            assert(variant_bin[i-1] == old_bin[i-1])
          end
        end)
        it("should not allow the variant proportions to be high", function()
          local j_table = json.decode(valid_json)
          j_table.Variants[1].percentage = "10"
          j_table.Variants[2].percentage = "70"
          local j_str = json.encode(j_table)
          local status, res = pcall(Tests.add, j_str, g_tests, c_index)
          assert(status == false)
          cleanup(g_tests, c_index)
        end)
        -- TODO check percentages match overall
      end)

      describe("for anonymous", function()
        it("should allow the variant proportions to be high", function()
          local j_table = json.decode(valid_json)
          j_table.external_id="123456789"
          j_table.Variants[1].percentage = "10"
          j_table.Variants[2].percentage = "70"
          j_table.BinType = "anonymous"
          local j_str = json.encode(j_table)
          local status, res = pcall(Tests.add, j_str, g_tests, c_index)
          assertx(status == true, res)
          cleanup(g_tests, c_index)
        end)
        -- TODO check percentages
        -- This is about dev specific routing too:
        describe("should add device specific routes", function()
          local j_table = json.decode(valid_json2)
          j_table.is_dev_specific = tostring(consts.TRUE)
          j_table.BinType = "anonymous"
          local j_str = json.encode(j_table)
          local status, res = pcall(Tests.add, j_str, g_tests, c_index)
          assertx(status == true, res)
          local dev_variants = j_table.DeviceCrossVariant
          table.sort(dev_variants, function(a,b) return a[0].device_id < b[0].device_id end)
          for index, variant in ipairs(dev_variants) do
            table.sort(variant, function(a,b) return a.id < b.id end)
          end
          it("Percentages should match with input", function()
            local bins = g_tests[c_index[0]].variant_per_bin
            local variants = g_tests[c_index[0]].variants
            local total = consts.AB_NUM_BINS
            for index, variant in ipairs(dev_variants) do
              local counts = {}
              for i=1,total do
                local idx = variant[i-1]
                local variant_id = tonumber(variants[idx].id)
                if counts[variant_id] == nil then
                  counts[variant_id] = 1
                else
                  counts[variant_id] = counts[variant_id] + 1
                end
              end
              local keys = 0
              for _, dev_spec_variants in pairs(devs_variants) do
                for _, variant in ipairs(dev_spec_variants) do
                  keys = keys + 1
                  local v_percentage = tonumber(variant.percentage)
                  local v_actual = counts[tonumber(variant.id)]
                  assert((v_actual - v_percentage < 0.1) or (v_percentage - v_actual< 0.1),
                    "Variation between request and actual should be less than 0.1%")
                end
              end
              assert(keys == #counts, "No keys should repeat")
            end
          end)


          cleanup(g_tests, c_index)
        end)

      end)
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
