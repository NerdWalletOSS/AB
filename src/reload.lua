-- local dbg = require 'debugger'
local ffi = require 'ab_ffi'
local sql = require 'sql'
local consts = require 'ab_consts'
local cache = require 'cache'
local load_config = require 'load_config'
local json = require 'json'
local AddTest = require 'add_test'
local reload = {}
function reload.get_tests_from_db()
  local tests = {}
  local configs = cache.get('config').AB.MYSQL
  local conn = load_config.db_connect(configs)
  local states = conn:query('select * from state;')
  local tmp = {}
  local valid_ids = {}
  for _,v in pairs(states) do
    local name = v.name:lower()
    if name == "started" or name == "terminated" then
      valid_ids[#valid_ids + 1] = v.id
      tmp[v.id] = name
    end
  end
  states = tmp

  tmp = {}
  local test_types = conn:query('select * from test_type;')
  for _,v in pairs(test_types) do
    tmp[v.id] = v.name
  end
  test_types = tmp

  tmp={}
  local creators = conn:query('select * from admin;')
  for _,v in pairs(creators) do
    tmp[v.id] = v.name
  end
  creators = tmp

  tmp = {}
  local bin_types = conn:query('select * from bin_type;')
  for _,v in pairs(bin_types) do
    tmp[v.id] = v.name
  end
  bin_types = tmp

  tmp={}
  local query_str = string.format('select * from test where state_id in (%s)', table.concat(valid_ids, ","))
  local tests = conn:query(query_str)
  for _,test in pairs(tests) do
    local test_json = {}
    test_json.Creator = creators[test.creator_id]
    test_json.TestType = test_types[test.test_type_id]
    test_json.BinType = bin_types[test.bin_type_id]
    test_json.State = states[test.state_id]
    -- TODO ask about channel stuff
    local query = string.format('select * from variant where test_id=%s;', test.id)
    local variants = conn:query(query)
    local count = 0
    local variants_arr = {}
    for  k,v in pairs(variants) do
      variants_arr[#variants_arr + 1] = v
    end
    test_json.Variants = variants_arr
    -- TODO ask about cat_attr_val_test
    -- TODO ask about bool_attr_test

    query = string.format('select * from device_x_variant where test_id=%s;', test.id)
    local devxvar = conn:query(query)
    if #devxvar > 0 then
      local devices = cache.get('devices')
      local dev_arr = {}
      for k,v in pairs(devices) do
        query = string.format('select * from device_x_variant where test_id=%s and device_id=%s', test.id, k)
        dev_arr[#dev_arr + 1] = conn:query(query)
      end
      test_json.DeviceCrossVariant = dev_arr
    end
    -- TODO here is where we send this to add test
    local str = json.encode(test_json)
    tests[#tests + 1] = str
  end
  return tests
end

function reload.reload(g_tests, c_index)
  local tests = reload.get_tests_from_db()
  assert(#tests > 0, "Should have at least one valid test")
  -- empty the cache and the c_struct
  ffi.fill(g_tests, ffi.sizeof("TEST_META_TYPE") * consts.AB_MAX_NUM_TESTS)
  c_index[0] = -1
  cache.put("tests", {})
  -- Now clean the tables entires in the c structure.
  for _, test in ipairs(tests) do
    c_index[0] = -1
    AddTest.add(test, g_tests, c_index)
  end
  c_index[0] = -1
end

return reload
