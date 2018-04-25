package.path=package.path .. ";./../src/?.lua"
local assertx = require 'assertx'
local dbg = require 'debugger'
local ffi = require 'ab_ffi'
local json = require 'json'
local consts = require 'ab_consts'
local AddTest = require 'add_test'
local spooky_hash = require 'spooky_hash'
local cache = require 'cache'
-- os.execute('make -C ../src libaux_zero.so')
-- local file = io.open("../src/aux_zero.h", "r")
-- ffi.cdef(file:read("*a"))
-- file:close()
--- local mallocer = ffi.load('../src/libaux_zero.so')
describe('AddTest framework', function()
  before_each(function()

  end)

  after_each(function()

  end)

  local g_tests = ffi.cast("TEST_META_TYPE*", ffi.C.malloc(ffi.sizeof("TEST_META_TYPE")*consts.AB_MAX_NUM_TESTS))
  local c_index = ffi.cast("int*", ffi.C.malloc(ffi.sizeof("int")))
  c_index[0] = -1
  local valid_json = require 'valid_json'
  local valid_json2 = require 'valid_json2'

  local function free_if_not_null(buff)
    if buff ~= nil then
      ffi.C.free(buff)
    end
    return nil
  end

  local function free_test(test)

  end

  local function free_variant_per_bin(test)
    local num_devices = 1
    if test.is_dev_specific == consts.TRUE then
      num_devices = 6 -- TODO remove hard coding
    end

    for i = 0, num_devices-1 do
      free_if_not_null(test.variant_per_bin[i])
    end
    free_if_not_null(test.variant_per_bin)
  end


  local function do_malloc_if_needed(g_tests, test_str)
    local o_arr = ffi.cast("int32_t*", ffi.gc(ffi.C.malloc(ffi.sizeof("int32_t")*4), ffi.C.free))
    -- TODO call preproc
    AddTest.preproc(test_str, g_tests, o_arr)
    -- TODO extract fields
    local num_variants = o_arr[2]
    local is_dev_specific = o_arr[3]
    local test_idx = o_arr[1]
    local state = o_arr[0]
    local num_devices = 6
    assert(num_variants >= consts.AB_MIN_NUM_VARIANTS and num_variants <= consts.AB_MAX_NUM_VARIANTS, "Invalid number of variants given")
    assert(is_dev_specific == consts.TRUE or is_dev_specific == consts.FALSE, "Device specific can only be true of false")
    assert(test_idx >=0 and test_idx < consts.AB_MAX_NUM_TESTS, "Invalid test index given")
    assert(state >=0 and state <= 9, "Invalid malloc of free strategy returned")
    if state == 7 then error("Terminated to started transition is not allowed") end
    if state == 8 then error("Terminated to terminated transition is not allowed") end

    local test = g_tests[test_idx]
    if state == 1 then
      test.variants = ffi.cast( "VARIANT_REC_TYPE*", ffi.gc(ffi.C.malloc(ffi.sizeof("VARIANT_REC_TYPE") * num_variants), ffi.C.free)) -- ffi malloc array of variants
      local url_length = consts.AB_MAX_LEN_VARIANT_URL+1
      local custom_data_length = consts.AB_MAX_LEN_CUSTOM_DATA+1
      for i=0, num_variants-1 do
        local variant = test.variants[i]
        test.variants[i].url = ffi.cast( "char*", ffi.gc(ffi.C.malloc(ffi.sizeof("char") * url_length), ffi.C.free))
        ffi.fill(test.variants[i].url, ffi.sizeof("char") * url_length)
        test.variants[i].custom_data = ffi.cast( "char*", ffi.gc(ffi.C.malloc(ffi.sizeof("char") * custom_data_length), ffi.C.free))
        ffi.fill(test.variants[i].custom_data, ffi.sizeof("char") * custom_data_length)
      end
      if is_dev_specific then -- started
        test.variant_per_bin = ffi.cast("uint8_t**", ffi.gc(ffi.C.malloc(ffi.sizeof("uint8_t*")*num_devices), ffi.C.free))
        ffi.fill(test.variant_per_bin, ffi.sizeof("uint8_t*")*num_devices)
        for index=0, num_devices-1 do
          test.variant_per_bin[index] = ffi.cast("uint8_t*", ffi.gc(
          ffi.C.malloc(ffi.sizeof("uint8_t")*consts.AB_NUM_BINS), ffi.C.free))
          ffi.fill(test.variant_per_bin[index] , ffi.sizeof("uint8_t")*consts.AB_NUM_BINS)
        end
      else
        test.variant_per_bin = ffi.cast("uint8_t**", ffi.gc(ffi.C.malloc(ffi.sizeof("uint8_t*")), ffi.C.free))
        ffi.fill(test.variant_per_bin, ffi.sizeof("uint8_t*"))
        test.variant_per_bin[0] = ffi.cast("uint8_t*", ffi.gc(
        ffi.C.malloc(ffi.sizeof("uint8_t")*consts.AB_NUM_BINS), ffi.C.free))
        ffi.fill(test.variant_per_bin[0] , ffi.sizeof("uint8_t")*consts.AB_NUM_BINS)
      end
    elseif state == 2 then -- terminated
      test.variants = ffi.cast( "VARIANT_REC_TYPE*", ffi.gc(ffi.C.malloc(ffi.sizeof("VARIANT_REC_TYPE") * num_variants), ffi.C.free)) -- ffi malloc array of variants
      local url_length = consts.AB_MAX_LEN_VARIANT_URL+1
      local custom_data_length = consts.AB_MAX_LEN_CUSTOM_DATA+1
      for i=0, num_variants-1 do
        local variant = test.variants[i]
        test.variants[i].url = ffi.cast( "char*", ffi.gc(ffi.C.malloc(ffi.sizeof("char") * url_length), ffi.C.free))
        ffi.fill(test.variants[i].url, ffi.sizeof("char") * url_length)
        test.variants[i].custom_data = ffi.cast( "char*", ffi.gc(ffi.C.malloc(ffi.sizeof("char") * custom_data_length), ffi.C.free))
        ffi.fill(test.variants[i].custom_data, ffi.sizeof("char") * custom_data_length)
      end

      if is_dev_specific then -- started
        test.final_variant_idx = ffi.cast("uint32_t*", ffi.gc(ffi.C.malloc(ffi.sizeof("uint32_t")*num_devices), ffi.C.free))
        test.final_variant_id = ffi.cast("uint32_t*", ffi.gc(ffi.C.malloc(ffi.sizeof("uint32_t")*num_devices), ffi.C.free))
      else
        test.final_variant_idx = ffi.cast("uint32_t*", ffi.gc(ffi.C.malloc(ffi.sizeof("uint32_t")), ffi.C.free))
        test.final_variant_id = ffi.cast("uint32_t*", ffi.gc(ffi.C.malloc(ffi.sizeof("uint32_t")), ffi.C.free))
      end

    elseif state == 3 then
      return
    elseif state == 4 then
      return
    elseif state == 5 then
      -- free variant per bin
      test.variant_per_bin = nil
    elseif state == 6 then
      -- free everything
      ffi.fill(g_tests + test_idx, ffi.sizeof("TEST_META_TYPE"))
    elseif state == 9 then
      -- free everything
      ffi.fill(g_tests + test_idx, ffi.sizeof("TEST_META_TYPE"))
    end


  end

  local function cleanup(g_tests, c_index)
    local index = tonumber(c_index[0])
    if index ~= -1 then
      ffi.fill(g_tests + index, ffi.sizeof("TEST_META_TYPE"))
    end
    local t = cache.get("tests")
    if t ~= nil then
      cache.put("tests", nil)
    end
  end

  local function fill_g_tests()
    for i=1,consts.AB_MAX_NUM_TESTS do
      g_tests[i-1].name_hash=1007
    end
  end

  local function empty_g_tests()
    ffi.fill(g_tests, ffi.sizeof("TEST_META_TYPE") * consts.AB_MAX_NUM_TESTS)
  end

  describe(", for ABTests ", function()
    describe('should add a test which', function()
      it("should work for a valid json string", function()
        local status, res = pcall(do_malloc_if_needed, g_tests, valid_json)
        assertx(status == true, "Malloc for valid test should succeed. Failure: ", res)
        status, res = pcall(AddTest.add, valid_json, g_tests, c_index)
        assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
        -- dbg()
        assert(c_index[0] ~= -1, "Entry should have a valid index")
        cleanup(g_tests, c_index)
      end)

      it("should have a valid TestType", function()
        local j_table = json.decode(valid_json)
        j_table.TestType = nil
        local j_str = json.encode(j_table)
        local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
        assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
        status, res = pcall(AddTest.add, j_str, g_tests, c_index)
        assert(status == false)
        cleanup(g_tests, c_index)
      end)

      it("should have a valid name", function()
        local j_table = json.decode(valid_json)
        j_table.name = nil
        local j_str = json.encode(j_table)
        -- dbg()
        local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
        assertx(status == false, "Insert for invalid test should fail")
        status, res = pcall(AddTest.add, j_str, g_tests, c_index)
        assert(status == false, res)
        cleanup(g_tests, c_index)
        -- dbg()


        j_table.name = string.rep("a", consts.AB_MAX_LEN_TEST_NAME + 1)
        j_str = json.encode(j_table)
        local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
        assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
        status, res = pcall(AddTest.add, j_str, g_tests, c_index)
        assert(status == false)
        cleanup(g_tests, c_index)



        j_table.name = string.rep("a", consts.AB_MAX_LEN_TEST_NAME)
        j_str = json.encode(j_table)
        local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
        assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
        status, res = pcall(AddTest.add, j_str, g_tests, c_index)
        assertx(status == true, "Insert for valid name should succeed Failure: ", res)
        assert(c_index[0] ~= -1, "Entry should have a valid index")
        cleanup(g_tests, c_index)
      end)

      it("should have a valid position", function()
        -- se default position 1 before and 1 after are only positions left
        local j_table = json.decode(valid_json)
        local default_pos = spooky_hash.spooky_hash64(j_table.name, #j_table.name, AddTest.g_seed1) % consts.AB_MAX_NUM_TESTS
        fill_g_tests()
        g_tests[default_pos].name_hash = 0
        local status, res = pcall(do_malloc_if_needed, g_tests, valid_json)
        assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
        local status, res = pcall(AddTest.add, valid_json, g_tests, c_index)
        assertx(status == true, "Insert should succeed in default position ", res)
        assert(c_index[0] == default_pos, "should insert in empty position")
        assert(c_index[0] ~= -1, "Entry should have a valid index")
        cleanup(g_tests, c_index)

        fill_g_tests()
        g_tests[default_pos + 1].name_hash = 0
        local status, res = pcall(do_malloc_if_needed, g_tests, valid_json)
        assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
        local status, res = pcall(AddTest.add, valid_json, g_tests, c_index)
        assert(status == true, "Insert should succeed in next to default position")
        assert(c_index[0] == default_pos + 1, "should insert in empty position")
        assert(c_index[0] ~= -1, "Entry should have a valid index")
        cleanup(g_tests, c_index)

        fill_g_tests()
        g_tests[default_pos - 1].name_hash = 0
        local status, res = pcall(do_malloc_if_needed, g_tests, valid_json)
        assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
        local status, res = pcall(AddTest.add, valid_json, g_tests, c_index)
        assert(status == true, "Insert should succeed in previous to default  position")
        assert(c_index[0] == default_pos - 1, "should insert in empty position")
        assert(c_index[0] ~= -1, "Entry should have a valid index")
        cleanup(g_tests, c_index)
        empty_g_tests()


        fill_g_tests()
        local status, res = pcall(do_malloc_if_needed, g_tests, valid_json)
        assertx(status == false, "Insert for valid test but full tests shoule result in failure. Failure: ", res)
        local status, res = pcall(AddTest.add, valid_json, g_tests, c_index)
        assert(status == false, "Insert should fail in a full tests array`")
        assert(c_index[0] == -1, "Entry should not exist")
        cleanup(g_tests, c_index)
        empty_g_tests()
      end)

      it("should allow updates to a test", function()
        local j_table = json.decode(valid_json)
        local status, res = pcall(do_malloc_if_needed, g_tests, valid_json)
        assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
        local status, res = pcall(AddTest.add, valid_json, g_tests, c_index)
        assert(status == true, res)
        local old_index = c_index[0]
        local old_testtype = g_tests[c_index[0]].test_type
        j_table.TestType = "XYTest"
        local j_str = json.encode(j_table)

        local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
        assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
        status, res = pcall(AddTest.add, j_str, g_tests, c_index)
        assert(status == true, res)
        assert(c_index[0] == old_index, "Indices for update must be the same. Expected", old_index, " got ", c_index[0] )

        assert(old_testtype ~= g_tests[c_index[0]].test_type, "Test type should have changed")

        cleanup(g_tests, c_index)

      end)
      describe("for c_to_v_ok_v_to_c_ok_v_to_v_not_ok", function()
        it("should have a control type", function()
          local j_table = json.decode(valid_json)
          j_table.Variants[1].name = 'failme'
          local j_str = json.encode(j_table)
          local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
          assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
          local status, res = pcall(AddTest.add, j_str, g_tests, c_index)
          assert(status == false)
          cleanup(g_tests, c_index)
        end)
        it('reordered variants should result in same ordering', function()
          local j_table = json.decode(valid_json)
          table.sort(j_table.Variants, function(a,b) return a.name > b.name end)
          local j_str = json.encode(j_table)
          local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
          assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
          local status, res = pcall(AddTest.add, j_str, g_tests, c_index)
          assert(status == true, 'Insertion for valid json should succeed')
          local variant_bin = g_tests[c_index[0]].variant_per_bin[0]
          local bin_size = ffi.sizeof(string.format("uint8_t[%s]", consts.AB_NUM_BINS))
          local old_bin = ffi.cast("uint8_t*", ffi.gc(ffi.C.malloc(bin_size), ffi.C.free))
          ffi.copy(old_bin , variant_bin, bin_size)
          cleanup(g_tests, c_index)

          table.sort(j_table.Variants, function(a,b) return a.name < b.name end)
          j_str = json.encode(j_table)
          local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
          assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
          status, res = pcall(AddTest.add, j_str, g_tests, c_index)
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
          local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
          assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
          local status, res = pcall(AddTest.add, j_str, g_tests, c_index)
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
          local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
          assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
          local status, res = pcall(AddTest.add, j_str, g_tests, c_index)
          assertx(status == true, res)
          cleanup(g_tests, c_index)
        end)
        -- TODO check percentages
        -- This is about dev specific routing too:
        describe("should add device specific routes", function()
          it("Percentages should match with input", function()
            local j_table = json.decode(valid_json2)
            j_table.is_dev_specific = tostring(consts.TRUE)
            j_table.BinType = "anonymous"
            j_table.State = "started"
            for _,v in ipairs(j_table.Variants) do
              v.url = "www.google.com"
            end
            local j_str = json.encode(j_table)
            local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
            assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
            -- dbg()
            local status, res = pcall(AddTest.add, j_str, g_tests, c_index)
            assertx(status == true, res)
            local dev_variants = j_table.DeviceCrossVariant
            table.sort(dev_variants, function(a,b) return a[0].device_id < b[0].device_id end)
            for index, variant in ipairs(dev_variants) do
              table.sort(variant, function(a,b) return a.id < b.id end)
            end
            local dev_counts = {}
            -- Desired counts
            for dev_name, device_variants in pairs(dev_variants) do
              local counts = {}
              for _, entry in ipairs(device_variants) do
                local variant_id = tonumber(entry.variant_id)
                local total = counts[variant_id] or 0
                total = total + tonumber(entry.percentage)
                counts[variant_id] = total
              end
              dev_counts[tonumber(device_variants[1].device_id)] = counts
            end

            -- Actual counts
            local size = #j_table.Variants
            local variant_index_to_id_map = {}
            for i=0, size -1 do
              local id = g_tests[c_index[0]].variants[i].id
              variant_index_to_id_map[i] = id
            end
            local bins = g_tests[c_index[0]].variant_per_bin
            local variants = g_tests[c_index[0]].variants
            local total = consts.AB_NUM_BINS
            local actual_counts = {}
            local num_dev_variants = 0
            for k,v in pairs(dev_variants) do
              num_dev_variants = num_dev_variants + 1
            end
            for bin_index=0,num_dev_variants -1 do
              local bin = bins[bin_index]
              local counts = {}
              for i=0, total - 1 do
                local v_id =  variant_index_to_id_map[bin[i]]
                counts[v_id] = (counts[v_id] or 0) + 1
              end
              actual_counts[bin_index + 1] = counts
            end


            -- Compare results
            for dev_id, counts in pairs(dev_counts) do
              local actual_count = assertx(actual_counts[dev_id], "must be present", dev_id)
              for v_id, count in pairs(counts) do
                local ac_index = v_id
                local ac = 0
                if ac_index ~= nil then
                  ac = (actual_count[v_id] or 0)/10
                end
                assertx(tonumber(count) == ac, "Must be the same number, expected ", count, " got ", ac, " for device_id ", dev_id, " variant number ", v_id)
              end
            end


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
                  local v_actual = counts[tonumber(variant.variant_id)]
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
  describe("should not have any final_variant entries for a started test", function()
    it("Should have a no entry for device agnosticc anonymous tests", function()
      local j_table = json.decode(valid_json2)
      j_table.is_dev_specific = tostring(consts.FALSE)
      j_table.BinType = "anonymous"
      j_table.State = "started"
      local j_str = json.encode(j_table)
      local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
      assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
      local status, res = pcall(AddTest.add, j_str, g_tests, c_index)
      local c_test = g_tests[c_index[0]]
      assert(c_test.final_variant_idx == nil, "Final variant idx should be nil")
      assert(c_test.final_variant_id == nil, "Final variant id should be nil")
      cleanup(g_tests, c_index)
    end)

    it("Should have a no entry for device agnosticc c_to_v_ok_v_to_c_ok_v_to_v_not_ok tests", function()
      local j_table = json.decode(valid_json2)
      j_table.is_dev_specific = tostring(consts.FALSE)
      j_table.BinType = "c_to_v_ok_v_to_c_ok_v_to_v_not_ok"
      j_table.State = "started"
      local j_str = json.encode(j_table)
      local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
      assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
      dbg()
      local status, res = pcall(AddTest.add, j_str, g_tests, c_index)
      local c_test = g_tests[c_index[0]]
      assert(c_test.final_variant_idx == nil, "Final variant idx should be nil")
      assert(c_test.final_variant_id == nil, "Final variant id should be nil")
      cleanup(g_tests, c_index)
    end)

    it("Should have no  entries for device specific anonymous tests", function()
      local j_table = json.decode(valid_json2)
      j_table.is_dev_specific = tostring(consts.TRUE)
      j_table.BinType = "anonymous"
      j_table.State = "started"
      local j_str = json.encode(j_table)
      local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
      assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
      local status, res = pcall(AddTest.add, j_str, g_tests, c_index)
      local num_devices = 0
      for _ in pairs(j_table.DeviceCrossVariant) do num_devices = num_devices + 1 end
      local c_test = g_tests[c_index[0]]
      assert(c_test.final_variant_idx == nil, "Final variant idx should be nil")
      assert(c_test.final_variant_id == nil, "Final variant id should be nil")
      cleanup(g_tests, c_index)
    end)
  end)

  describe("should have any final_variant entries for a terminated test", function()
    it("for device agnostic anonymous tests", function()
      local j_table = json.decode(valid_json2)
      j_table.is_dev_specific = tostring(consts.FALSE)
      j_table.BinType = "anonymous"
      j_table.State = "terminated"
      j_table.Variants[1].is_final = tostring(consts.TRUE)
      local j_str = json.encode(j_table)
      local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
      assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
      local status, res = pcall(AddTest.add, j_str, g_tests, c_index)
      local c_test = g_tests[c_index[0]]
      assert(c_test.final_variant_idx ~= nil, "Final variant idx should not be nil")
      assert(c_test.final_variant_id ~= nil, "Final variant id should not be nil")
      assert(c_test.final_variant_idx[0] ~= nil, "Final variant idx should not be nil")
      assert(c_test.final_variant_id[0] ~= nil, "Final variant id should not be nil")


      cleanup(g_tests, c_index)
    end)

    it("for device agnostic c_to_v_ok_v_to_c_ok_v_to_v_not_ok tests", function()
      local j_table = json.decode(valid_json2)
      j_table.is_dev_specific = tostring(consts.FALSE)
      j_table.BinType = "c_to_v_ok_v_to_c_ok_v_to_v_not_ok"
      j_table.State = "terminated"
      j_table.Variants[1].is_final = tostring(consts.TRUE)
      local j_str = json.encode(j_table)
      local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
      assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
      local status, res = pcall(AddTest.add, j_str, g_tests, c_index)
      assert(status, res)
      local c_test = g_tests[c_index[0]]
      assert(c_test.final_variant_idx ~= nil, "Final variant idx should not be nil")
      assert(c_test.final_variant_id ~= nil, "Final variant id should not be nil")
      assert(c_test.final_variant_idx[0] ~= nil, "Final variant idx should not be nil")
      assert(c_test.final_variant_id[0] ~= nil, "Final variant id should not be nil")


      cleanup(g_tests, c_index)
    end)

    it("for device specific anonymous tests", function()
      local j_table = json.decode(valid_json2)
      j_table.is_dev_specific = tostring(consts.TRUE)
      j_table.BinType = "anonymous"
      j_table.State = "terminated"
      j_table.Variants[1].is_final = tostring(consts.TRUE)
      local j_str = json.encode(j_table)
      local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
      assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
      local status, res = pcall(AddTest.add, j_str, g_tests, c_index)
      local num_devices = 0
      for _ in pairs(j_table.DeviceCrossVariant) do num_devices = num_devices + 1 end
      local c_test = g_tests[c_index[0]]
      assert(c_test.final_variant_idx ~= nil, "Final variant idx should not be nil")
      assert(c_test.final_variant_id ~= nil, "Final variant id should not be nil")
      for index = 0 , num_devices - 1 do
        assert(c_test.final_variant_idx[index] ~= nil, "Final variant idx should not be nil")
        assert(c_test.final_variant_id[index] ~= nil, "Final variant id should not be nil")
      end

      cleanup(g_tests, c_index)
    end)
  end)



  describe("should delete archived tests", function()
    empty_g_tests()
    local status, res = pcall(do_malloc_if_needed, g_tests, valid_json)
    assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
    local status, res = pcall(AddTest.add, valid_json, g_tests, c_index)
    assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
    assert(c_index[0] ~= -1, "Entry should have a valid index")
    local j_table = json.decode(valid_json)
    j_table.State = "archived"
    local j_str = json.encode(j_table)
    local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
    assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
    do_malloc_if_needed(g_tests, j_str)
    status, res = pcall(AddTest.add, j_str, g_tests, c_index)
    assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
    assert(c_index[0] ~= -1, "Entry should have a valid index")
    assert(g_tests[c_index[0]].id == 0, 'expect 0 as id  when nulled out')
    assert(ffi.string(g_tests[c_index[0]].name) == '', 'expect an empty string when nulled out')
  end)
  -- describe("for XYTests", function()
  -- end)

  describe("should fail for any other testtype", function()
    local j_table = json.decode(valid_json)
    j_table.TestType = "InvalidTest"
    local j_str = json.encode(j_table)
    local status, res = pcall(do_malloc_if_needed, g_tests, j_str)
    assertx(status == true, "Insert for valid test should succeed. Failure: ", res)
    local status, res = pcall(AddTest.add, j_str, g_tests, c_index)
    assert(status == false)
    cleanup(g_tests, c_index)
  end)
end)
