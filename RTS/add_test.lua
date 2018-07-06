local dbg = require 'lua/debugger'
local JSON = require 'lua/JSON'
local cache = require 'lua/cache'
local assertx = require 'lua/assertx'
local ffi = require 'lua/ab_ffi'
local AddTests = {}
local bins = require 'RTS/bins'
local consts = require 'lua/ab_consts'
local g_seed1 = 961748941 -- TODO remove manual copy from zero_globals.c
local g_seed2 = 982451653 -- TODO remove manual copy from zero_globals.c
AddTests.g_seed1 = g_seed1
AddTests.g_seed2 = g_seed2
local spooky_hash = require 'RTS/spooky_hash'

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

local function get_test_type(TestType)
  assert(TestType ~= nil, "Test type cannot be nil")
  local x = tonumber(TestType)
  if x ~= nil then return x end
  if TestType:lower() == "abtest" then
    return consts.AB_TEST_TYPE_AB
  elseif TestType:lower() == "xytest" then
    return consts.AB_TEST_TYPE_XY
  else
    error("Invalid test type given")
  end
end

local function get_pos(g_tests, test_data,  test_type, c_index, name_hash, match_func)
  local original_position = tonumber(name_hash % consts.AB_MAX_NUM_TESTS)
  g_tests = ffi.cast("TEST_META_TYPE*", g_tests)
  local position = original_position
  print("original position", position)
  local test = nil
  local stop = false
  repeat
    test = ffi.cast("TEST_META_TYPE*", g_tests)[position]
    position = position + 1
    if position == consts.AB_MAX_NUM_TESTS or  match_func(test, test_data, name_hash) then
      stop = true
    end
  until stop == true
  if match_func(test, test_data, name_hash) then
    c_index[0] = position -1
    return test, name_hash
  else
    -- Now search from 0 to orignal position -1
    position = 0
    stop = false
    repeat
      test = ffi.cast("TEST_META_TYPE*", g_tests)[position]
      position = position + 1
      if (position - 1)== original_position or match_func(test, test_data, name_hash)then
        stop = true
      end
    until stop == true
    if match_func(test, test_data, name_hash) then
      -- test.name_hash = name_hash
      -- print(position -1)
      c_index[0] = position - 1
      return test, name_hash
    else
      c_index[0] = -1
      return -1
    end
  end
end

local function is_pos_empty(c_test)
  if c_test.name_hash == 0 then
    return true
  end
end

local function match(c_test, test_data, name_hash)
  if c_test.name_hash == name_hash and c_test.id == tonumber(test_data.id) and ffi.string(c_test.name) == test_data.name then
    return true
  else
    return false
  end
end

function AddTests.get_test(
  g_tests,
  test_data,
  c_index
  )
  -- print("name is ", test_data.name)
  local name_hash = spooky_hash.spooky_hash64(test_data.name, #test_data.name, g_seed1)
  local position = name_hash % consts.AB_MAX_NUM_TESTS
  local original_position = position
  c_index = ffi.cast("int*", c_index)
  local test_type = assert(get_test_type(test_data.TestType), "TestType cannot be nil")
  print("pos0")
  local test = get_pos(g_tests, test_data, test_type, c_index, name_hash, match)
 print("pos1", c_index[0])
  if test == -1 then
    test = get_pos(g_tests, test_data, test_type, c_index, name_hash, is_pos_empty)
     print("pos2", c_index[0])
  end
  assert(test ~= -1 , "Unable to find an empty spot")
  return ffi.cast("TEST_META_TYPE*", g_tests)[c_index[0]], name_hash
end


function AddTests.add(
  test_str,
  g_tests,
  c_index
  )
  -- print(test_str)
  c_index = ffi.cast("int*", c_index)
  local test_data = JSON:decode(test_str)
  assert(type(test_data) == "table", "Unable to decode json string")
  assert(type(test_data) == "table", "Invalid JSON string given")
  -- dbg()
  local test_type = assert(get_test_type(test_data.TestType), "TestType cannot be nil")
  assert(test_data.name ~= nil, "Test should have a name")
  assertx(#test_data.name <= consts.AB_MAX_LEN_TEST_NAME,
  "Test name should be below test name limit. Got ", #test_data.name,
  " Expected max ", consts.AB_MAX_LEN_TEST_NAME)
  local c_test, name_hash = AddTests.get_test(g_tests, test_data, c_index)
  c_test.name_hash = name_hash
  assert(c_test.variants, "no space allocated for variants")

  assert(c_test ~= nil, "Position not found to insert")
  print("TestState", test_data.State:lower())
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
    if ( type(seed) == "number" ) then
      seed = tostring(seed)
    end
    c_test.seed = spooky_hash.convert_str_to_u64(seed)
    local is_dev_spec = assert(tonumber(test_data.is_dev_specific), "Must have boolean device specific routing")
    assert(is_dev_spec == consts.TRUE or is_dev_spec == consts.FALSE, "Device specific must have TRUE or FALSE values only")
    c_test.test_type = get_test_type(test_type)
    -- print(c_index[0])
    -- dbg()
    assert(c_test.variants ~= nil, "Space must be allocated for variants")
    -- dbg()
    bins[test_data.BinType].add_bins_and_variants(c_test, test_data)
    local tests = cache.get("tests") or {}
    tests[test_data.id] = test_data
    cache.put("tests", tests)
  end
end

function AddTests.preproc(test_str, g_tests, o_arr)
  -- o arr is the array that returns requirements to C
  -- int what_to_do      = rslt[0];
  -- int test_idx        = rslt[1];
  -- int num_variants     = rslt[2];
  -- int is_dev_specific = rslt[3];
  --
  local j_table = JSON:decode(test_str)
  assert(type(j_table) == "table", "Unable to decode json string")
  g_tests = ffi.cast("TEST_META_TYPE*", g_tests)
  o_arr = ffi.cast("int*", o_arr)
  AddTests.get_test(g_tests, j_table, o_arr + 1) -- get test position iin 0 location or o_arr
  o_arr[2] = #j_table.Variants
  o_arr[3] = tonumber(j_table.is_dev_specific)
  local tests = cache.get("tests") or {}
  local test = tests[j_table.id]
  local state = j_table.State:lower()
  tests = nil
  if test == nil then
    if state ==  "started" then
      o_arr[0] = 1
    elseif state == "terminated" then
      o_arr[0] = 2
    elseif state == "archived" then
      o_arr[0] = 3
    else
      error("Invalid state present: " ..state)
    end
    return
  else
    local old_state = test.State:lower()
    if old_state == "started" then
      if state ==  "started" then
        o_arr[0] = 4
      elseif state == "terminated" then
        o_arr[0] = 5
      elseif state == "archived" then
        o_arr[0] = 6
      else
        error("Invalid state present: " ..state)
      end
      return
    elseif old_state == "terminated" then
      if state ==  "started" then
        o_arr[0] = 7
      elseif state == "terminated" then
        o_arr[0] = 8
      elseif state == "archived" then
        o_arr[0] = 9
      else
        error("Invalid state present: " ..state)
      end
      return
    else
      error("Invali old state " .. old_state)
    end
    -- dbg()
  end
  -- no change in num variants or is_dev_specific
end

return AddTests
