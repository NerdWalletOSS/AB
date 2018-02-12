-- local dbg = require 'debugger'
local json = require 'json'
local assertx = require 'assertx'
local ffi = require 'ab_ffi'
local Tests = {}
local consts = require 'ab_consts'
local assertx = require 'assertx'
local g_seed1 = 961748941 -- TODO remove manual copy
Tests.g_seed1 = g_seed1
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

local function set_variants_per_bin(c_test, variant_count)
  --clean all the entries
  -- TODO account for devices later
  local c_type = string.format("uint8_t[%s]", consts.AB_NUM_BINS)
  local cast_type = string.format("uint8_t *(&)[%s]", consts.AB_NUM_BINS)
  c_test.variant_per_bin = ffi.cast(cast_type, ffi.gc(
  ffi.C.malloc(ffi.sizeof(cast_type)), ffi.C.free))

  -- print(ffi.typeof(c_test.variant_per_bin[0]))
  c_test.variant_per_bin[0] = ffi.cast("uint8_t*", ffi.gc(
  ffi.C.malloc(ffi.sizeof(c_type)), ffi.C.free))

  local variants = c_test.variants
  -- print("variant 0", ffi.string(variants[0].name):lower())
  assert(ffi.string(variants[0].name):lower() == "control", "The first entry should always be control")
  -- TODO expand to per device later
  local variants_bin = c_test.variant_per_bin[0]
  ffi.fill(variants_bin, ffi.sizeof(c_type))


  -- First use dedicated bins for each variant
  -- Dedicated bins for variant i are i, i+1*nV, i+2*nV ...
  for index = 1,variant_count -1 do -- Dont care about 0 index as that is control
    local variant = variants[index]
    local total_bins = math.floor(variant.percentage * 0.01 * consts.AB_NUM_BINS)
    local num_set = 0
    for set_index=index,consts.AB_NUM_BINS-1 ,variant_count do
      -- If target is reached then stop else claim the bin
      if num_set ~= total_bins then
        num_set = num_set + 1
        variants_bin[set_index] = index -- set_indexth bucket points to indexth variant
      end
    end
    if num_set ~= total_bins then
      -- If we dont have adequate bins we get buckets from control
      -- A variant can only get a bucket from control if (j/nV)%(i-1) == 0
      -- So, if nV = 3,
      -- Control's bins are 0, 3, 6, 9, 12, 15, ... These are
      -- renumbered by dividing by 3:  0, 1, 2, 3, 4,  5,
      --Variant 1 gets renumbered bins 0, 2, 4, ....
      --Variant 1 gets original   bins 0, 6, 12, ....
      --Variant 2 gets renumbered bins 1, 3, 5, ...
      --Variant 2 gets original   bins 3, 9, 15, ...
      local start = (index-1)*variant_count
      local incr = (variant_count-1)*variant_count
      for set_index=start, consts.AB_NUM_BINS -1 , incr do
        if num_set ~= total_bins then
          variants_bin[set_index] = ffi.cast("uint8_t", index)
          num_set = num_set + 1
        end
      end
    end
  end
end

local function add_variants(c_test, json_table)
  local variants = assert(json_table.Variants, "Test should have variants")
  assert(type(variants) == "table", "Variants should be an array of variants")
  assertx(#variants >= consts.AB_MIN_NUM_VARIANTS and #variants <=consts.AB_MAX_NUM_VARIANTS,
  "Expected variants to be between ", consts.AB_MIN_NUM_VARIANTS)
  c_test.num_variants = #variants
  c_test.variants = ffi.cast( "VARIANT_REC_TYPE*", ffi.gc(
  ffi.C.malloc(ffi.sizeof("VARIANT_REC_TYPE") * #variants), ffi.C.free)) -- ffi malloc array of variants
  -- c_test.final_variant_id = assert(json_table.FinalVariantID, "need a final variant") -- TODO check where this comes in
  local pos, curr_index, total = 1, 0, 0
  for _, value in ipairs(variants) do
    local entry
    if value.name:lower() == "control" then
      entry = c_test.variants[0]
      curr_index = 0
    else
      entry = c_test.variants[pos]
      curr_index = pos
      pos = pos + 1
    end
    entry.id = assert(tonumber(value.id), "Expected to have entry for id of variant")
    if entry.id == c_test.final_variant_id then
      c_test.final_variant_idx = curr_index
    end
    entry.percentage = assert(tonumber(value.percentage), "Every variant must have a percentage")
    total = total + entry.percentage
    -- print(curr_index, value.name)
    assertx(value.name and #value.name<= consts.AB_MAX_LEN_VARIANT_NAME, "Valid name for variant at position " , index)
    ffi.copy(entry.name, value.name)
    entry.url = assert(value.url, "Entry should have a url") -- TODO why do we have a max length?
    entry.custom_data = value.custom_data or "NULL" -- TODO why do we have a max length
  end
  assertx(total == 100, "all the percentages should add up to 100, added to ", total)
  set_variants_per_bin(c_test, #variants)
end

function Tests.get_test(g_tests, test_name, c_index)
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
  if test.name_hash == 0 or test.name_hash == name_hash then
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
      if position == original_position or test.name_hash == 0 or test.name_hash == name_hash then
        stop = true
      end
    until stop == true
    if test.name_hash == 0 or test.name_hash == name_hash then
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
  local c_test = Tests.get_test(g_tests, test_data.name, c_index)
  assert(c_test ~= nil, "Position not found to insert")
  ffi.copy(c_test.name, test_data.name)
  c_test.test_type = consts.AB_TEST_TYPE_AB
  c_test.state = assert(states[test_data.State], "Must have a valid state")
  c_test.id = assert(tonumber(test_data.id), "Must have a valid test id")
  local seed = assert(test_data.seed, "Seed needs to be specified for test")
  c_test.seed = spooky_hash.convert_str_to_u64(seed)
  if test_type == "ABTest" then
    c_test.is_dev_specific = consts.FALSE
    -- c_test.has_filters -- TODO boolean value of 0 or 1 only for AB
    add_variants(c_test, test_data)
    -- c_test.dev_spec_perc -- TODO device specific to variants or this
    -- c_test.n_dev_spec_per -- TODO device specific to variants or thisc

  elseif test_type == "XYTest" then
    -- c_test.is_dev_specific -- TODO boolean value of 0 or 1 only for AB
    local external_id = assert(test_data.external_id, "External id needs to be specified for test")
    c_test.external_id = spooky_hash.convert_str_to_u64(external_id)

  else
    error("Tests can only be of type ABTest or XYTest")
  end
end

return Tests
