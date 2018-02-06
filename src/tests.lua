-- local dbg = require 'debugger'
local json = require 'json'
local assertx = require 'assertx'
local ffi = require 'ab_ffi'
local Tests = {}
local consts = require 'ab_consts'
local assertx = require 'assertx'
local g_seed1 = 961748941 -- TODO remove manual copy
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

local function add_variants(c_test, json_table)
  local variants = assert(json_table.Variants, "Test should have variants")
  assert(type(variants) == "table", "Variants should be an array of variants")
  assertx(#variants >= consts.AB_MIN_NUM_VARIANTS and #variants <=consts.AB_MAX_NUM_VARIANTS,
  "Expected variants to be between ", consts.AB_MIN_NUM_VARIANTS)
  c_test.num_variants = #variants
  c_test.variants = ffi.cast( "VARIANT_REC_TYPE*", ffi.gc(
  ffi.C.malloc(ffi.sizeof("VARIANT_REC_TYPE") * #variants), ffi.C.free)) -- ffi malloc array of variants
  -- c_test.final_variant_id = assert(json_table.FinalVariantID, "need a final variant") -- TODO check where this comes in
  local total = 0
  for index, value in ipairs(variants) do
    local entry = c_test.variants[index -1]
    entry.id = assert(tonumber(value.id), "Expected to have entry for id of variant")
    if entry.id == c_test.final_variant_id then
      c_test.final_variant_idx = index - 1
    end
    entry.percentage = assert(tonumber(value.percentage), "Every variant must have a percentage")
    total = total + entry.percentage
    -- TODO check sum is == 100
    assertx(value.name and #value.name<= consts.AB_MAX_LEN_VARIANT_NAME, "Valid name for variant at position " , index)
    ffi.copy(entry.name, value.name)
    entry.url = assert(value.url, "Entry should have a url") -- TODO why do we have a max length?
    entry.custom_data = value.custom_data or "" -- TODO why do we have a max length
  end
  assertx(total == 100, "all the percentages should add up to 100, added to ", total)
end

local function get_test(g_tests, test_name, c_index)
  local name_hash = spooky_hash.spooky_hash64(test_name, #test_name, g_seed1)
  local position = name_hash % consts.AB_MAX_NUM_TESTS
  local original_position = position
  c_index = ffi.cast("int*", c_index)
  g_tests = ffi.cast("TEST_META_TYPE*", g_tests)
  local test = nil
  local stop = false
  repeat
    test = ffi.cast("TEST_META_TYPE*", g_tests)[position]
    position = position + 1
    if position == consts.AB_MAX_NUM_TESTS or test.name_hash == 0 or test.name_hash == name_hash then
      stop = true
    end
  until stop == true
  if position <= consts.AB_MAX_NUM_TESTS and (test.name_hash == 0 or test.name_hash == name_hash) then
    test.name_hash = name_hash
    -- print(position -1)
    c_index[0] = position -1
    return test
  else
    -- Now search from 0 to orignal position -1
    position = 0
    repeat
      test = ffi.cast("TEST_META_TYPE*", g_tests)[position]
      position = position + 1
      if position == original_position or test.name_hash == 0 or test.name_hash == name_hash then
        stop = true
      end
    until stop == true
    if position <= original_position and (test.name_hash == 0 or test.name_hash == name_hash) then
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


function Tests.add(test_str, g_tests, c_index)
  -- print(test_str)
  local test_data = json.decode(test_str)
  local test_type = assert(test_data.TestType, "TestType cannot be nil for test")
  assert(test_data.name ~= nil, "Test should have a name")
  assertx(#test_data.name <= consts.AB_MAX_LEN_TEST_NAME,
  "Test name should be below test name limit. Got ", #test_data.name,
  " Expected max ", consts.AB_MAX_LEN_TEST_NAME)
  if test_type == "ABTest" then
    local c_test = get_test(g_tests, test_data.name, c_index)
    assert(c_test ~= nil, "Position not found to insert")
    c_test = ffi.cast("TEST_META_TYPE*", c_test)[0]
    ffi.copy(c_test.name, test_data.name)
    c_test.test_type = consts.AB_TEST_TYPE_AB
    c_test.state = assert(states[test_data.State], "Must have a valid state")
    c_test.id = assert(tonumber(test_data.id), "Must have a valid test id")
    -- c_test.name_hash = spooky_hash.spooky_hash64(c_test.name, ffi.C.strlen(c_test.name), g_seed1) -- TODO remove spooky from ffi
    -- c_test.external_id -- TODO onlt for XY Test
    -- c_test.has_filters -- TODO boolean value of 0 or 1 only for AB
    -- c_test.is_dev_specific -- TODO boolean value of 0 or 1 only for AB
    -- c_test.state -- TODO started updated terminated or deleted
    -- c_test.seed -- TODO have to ask about this one
    add_variants(c_test, test_data)
    -- c_test.variant_per_bin [AB_NUM_BINS] -- TODO have to ask about this one
    -- c_test.dev_spec_perc -- TODO device specific to variants or this
    -- c_test.n_dev_spec_per -- TODO device specific to variants or thisc

    -- c_struct[position] = data
  elseif test_type == "XYTest" then

  else
    error("Tests can only be of type ABTest or XYTest")
  end
end

return Tests
