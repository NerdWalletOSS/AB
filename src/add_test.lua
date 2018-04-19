-- local dbg = require 'debugger'
local json = require 'json'
local cache = require 'cache'
local assertx = require 'assertx'
local ffi = require 'ab_ffi'
local AddTests = {}
local bins = require 'bins'
local consts = require 'ab_consts'
local assertx = require 'assertx'
local g_seed1 = 961748941 -- TODO remove manual copy
AddTests.g_seed1 = g_seed1
local spooky_hash = require 'spooky_hash'

local function create_state_table(consts)
  local states = {}
  for k,v in pairs(consts) do
    local val = nil
    for match in k:gmatch("TEST_STATE_(%w+)") do
      val = match
    end
    if val ~= nil then
      states[val:lower()] = v
      -- print(val:lower(), v)
    end
  end
  return states
end

local states = create_state_table(consts)

local function match(c_test, test_data, name_hash)
  if c_test.name_hash == 0 then
    return true
  elseif c_test.name_hash == name_hash and c_test.id == tonumber(test_data.id) and ffi.string(c_test.name) == test_data.name then
    return true
  end
end

function AddTests.get_test(g_tests, test_data, c_index)
  -- dbg()
  local name_hash = spooky_hash.spooky_hash64(test_data.name, #test_data.name, g_seed1)
  local position = name_hash % consts.AB_MAX_NUM_TESTS
  local original_position = position
  c_index = ffi.cast("int*", c_index)
  g_tests = ffi.cast("TEST_META_TYPE*", g_tests)
  local test = nil
  local stop = false
  repeat
    test = ffi.cast("TEST_META_TYPE*", g_tests)[position]
    position = position + 1
    if position == consts.AB_MAX_NUM_TESTS or  match(test, test_data, name_hash) then
      stop = true
    end
  until stop == true
  if match(test, test_data, name_hash) then
    test.name_hash = name_hash
    -- print(position -1)
    c_index[0] = position -1
    return test
  else
    -- Now search from 0 to orignal position -1
    position = 0
    stop = false
    repeat
      test = ffi.cast("TEST_META_TYPE*", g_tests)[position]
      position = position + 1
      if position == original_position or match(test, test_data, name_hash)then
        stop = true
      end
    until stop == true
    if match(test, test_data, name_hash) then
      test.name_hash = name_hash
      -- print(position -1)
      c_index[0] = position - 1
      return test
    else
      c_index[0] = -1
      assert(nil, "Unable to find any position for insertion")
    end
  end
end


function AddTests.add(test_str, g_tests, c_index)
  -- print(test_str)
  local test_data = json.decode(test_str)
  local test_type = assert(test_data.TestType, "TestType cannot be nil for test")
  assert(test_data.name ~= nil, "Test should have a name")
  assertx(#test_data.name <= consts.AB_MAX_LEN_TEST_NAME,
  "Test name should be below test name limit. Got ", #test_data.name,
  " Expected max ", consts.AB_MAX_LEN_TEST_NAME)
  local c_test = AddTests.get_test(g_tests, test_data, c_index)
  assert(c_test ~= nil, "Position not found to insert")
  if test_data.State:lower() == "archived" then
    -- delete the test
    -- delete from cache
    ffi.fill( ffi.cast("TEST_META_TYPE*", g_tests) + c_index[0], ffi.sizeof("TEST_META_TYPE"))
    local tests = cache.get("tests") or {}
    tests[test_data.id] = nil
    cache.put("tests", tests)

  else
    ffi.copy(c_test.name, test_data.name)
    c_test.test_type = consts.AB_TEST_TYPE_AB
    c_test.state = assert(states[test_data.State], "Must have a valid state")
    c_test.id = assert(tonumber(test_data.id), "Must have a valid test id")
    local seed = assert(test_data.seed, "Seed needs to be specified for test")
    c_test.seed = spooky_hash.convert_str_to_u64(seed)
    local is_dev_spec = assert(tonumber(test_data.is_dev_specific), "Must have boolean device specific routing")
    assert(is_dev_spec == consts.TRUE or is_dev_spec == consts.FALSE, "Device specific must have TRUE or FALSE values only")
    if test_type == "ABTest" then
      c_test.test_type = consts.AB_TEST_TYPE_AB
    elseif test_type == "XYTest" then
      c_test.test_type = consts.AB_TEST_TYPE_XY
    else
      error("Invalid TestType given")
    end
    -- dbg()
    bins[test_data.BinType].add_bins_and_variants(c_test, test_data)
    local tests = cache.get("tests") or {}
    tests[test_data.id] = test_data
    cache.put("tests", tests)
  end
end

function AddTests.preproc(test_str, g_tests, o_arr)
  -- o arr is the array that returns requirements to C
  -- 0 is the test id
  -- 1 is the number of variants
  -- 2 is wheter it is device specific or not
  -- 3 is whether it is terminated or not
  local j_table = json.decode(test_str)
  o_arr = ffi.cast("int32_t*", o_arr)
  AddTests.get_test(g_tests, test_data, o_arr) -- get test position iin 0 location or o_arr
  o_arr[1] = #j_table.Variants
  o_arr[2] = tonumber(j_table.is_dev_specific)
  if j_table.State:lower() == "terminated" then
    o_arr[3] = consts.TRUE
  else
    o_arr[3] = consts.FALSE
  end
end
return AddTests
