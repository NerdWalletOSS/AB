require 'busted.runner'()
-- package.path=package.path .. ";./../src/?.lua"
local cache = require 'lua/cache'
local consts = require 'lua/ab_consts'
local mysql = require 'lua/sql'
-- local AddTest = require 'RTS/add_test'
-- local config_str = require 'valid_config'
local config_path = '../src/ab.conf.json'
local file = io.open('../src/ab.conf.json', 'r')
local conf_str = file:read('*a')
file:close()

local ffi = require 'lua/ab_ffi'
local load_config = require 'RTS/ab_load_config'
-- local dbg = require 'lua/debugger'
local reload = require 'RTS/reload'
local assertx = require 'lua/assertx'

local function is_same(a, b)
  local val = type(a) == type(b)
  if not val then return false end
  if type(a) == "table" then
    for k,v in pairs(a) do
      if not is_same(v, b[k]) then return false end
    end
    for k,v in pairs(b) do
      if not is_same(v, a[k]) then return false end
    end
    return true
  else
    return a == b
  end
end

local function load_all_configs()
  local confs_size = consts.NUM_SERVICES
  local g_conf = ffi.cast("CFG_TYPE*", ffi.gc(ffi.C.malloc(ffi.sizeof("CFG_TYPE")), ffi.C.free))
  local has_changed = ffi.cast("unsigned char*", ffi.gc(ffi.C.malloc(ffi.sizeof("unsigned char")* confs_size), ffi.C.free))
  ffi.fill(g_conf, ffi.sizeof("CFG_TYPE"))
  ffi.fill(has_changed, ffi.sizeof("unsigned char")*confs_size )
  return load_config.load_config(config_path, g_conf, has_changed)
end

describe('Reload ', function()
  cache.put('config', load_all_configs())
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
  local query_str = string.format('select * from test where state_id in (%s)', table.concat(valid_ids, ","))
  local tests = conn:query(query_str)
  it('should return entries for the dbs that are valid', function()
    local reload_entries = reload.get_tests_from_db()
    -- dbg()
    assertx(#tests == #reload_entries, "Expected tests to have ",#tests, "entries, got", #reload_entries)
  end)
  -- describe('should match the entries returned by php', function()
  -- end)
  -- now reload calls add tests in C lua just returns numbwer of tests
  -- it('should re populate the c data structure', function()
  --   local g_tests = ffi.cast("TEST_META_TYPE*", ffi.gc(ffi.C.malloc(ffi.sizeof("TEST_META_TYPE")*consts.AB_MAX_NUM_TESTS), ffi.C.free))
  --   local c_index = ffi.cast("int*", ffi.gc(ffi.C.malloc(ffi.sizeof("int")), ffi.C.free))
  --   c_index[0] = -1
  --   local status, res = pcall(reload.reload, g_tests, c_index)
  --   assertx(status == true, "Reload should work, got error: ", res)
  --   for _, test in ipairs(tests) do
  --     -- perform a naieve scan over the array and match the id and name
  --     local idx = -1
  --     for i=1,consts.AB_MAX_NUM_TESTS -1 do
  --       if idx == -1 then
  --         local c_test = g_tests[i]
  --         if c_test.id == tonumber(test.id) and ffi.string(c_test.name) == test.name then
  --           idx = i
  --         end
  --       end
  --     end
  --     assert(idx ~= -1, "Every test must be found in the c data structure")
  --     ffi.fill(g_tests + idx, ffi.sizeof("TEST_META_TYPE"))
  --   end
  --   -- There should be no more tests in the tests c structure
  --   for i=1,consts.AB_MAX_NUM_TESTS -1 do
  --     local c_test = g_tests[i]
  --     assertx(c_test.id == 0 and ffi.string(c_test.name) == "", "No entry expected, found entry at index ", i)
  --   end
  -- end)
end)

