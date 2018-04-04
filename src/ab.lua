local csv = require "csv"
local json = require "json"
-- local ffi = require 'ffi' 
local tests = require 'add_test'
local load_cfg = require 'load_config'
local cache = require 'cache'
local reload_tests = require 'reload'

function add(...)
  tests.add(...)
end

--=== Sample of what you need to do to get a new Lua file in
--=== for any Lua function that is pcall'ed from C. As an example
--===  lua_getglobal(g_L, "make_feature_vector");
--===  if ( !lua_isfunction(g_L, -1)) {
local xxx = require 'make_feature_vector'
function make_feature_vector(...)
  return xxx(...)
end

function load_config(...)
  load_cfg.load_config_from_file(...)
end

function list_tests()
  return json.encode(cache.get('tests'))
end

function get_config()
  return cache.get('config')
end

function reload(...)
  return reload_tests.reload(...)
end

function check_db_conn()
  local configs = cache.get('config').AB.MYSQL
  local conn = load_config.db_connect(configs)
  assert(conn ~= nil, "must be a valid connection object")
end

function get_config()
  return json.encode(cache.get('config'))
end

function get_test_info(args)
  local j_table = json.decode(args)
  local tests = cache.get('tests')
  local test = nil
  if j_table.id ~= nil then
    test = tests[j_table.id]
    assert(test ~= nil, "Tests with id ", j_table.id, " does not exist")
  elseif j_table.name ~= nil and j_table.test_type ~= nil then
    for _, v in pairs(tests) do
      if v.name == j_table.name and v.test_type == j_table.test_type then
        assert(test == nil, "More than one match exists")
        test = v
      end
    end
  end
  return json.encode(test)
end

-- function add(c_str, c_data)
--    -- print("hi from lua", c_str)
--    local x = ffi.cast("int*", c_data)
--    print(c_str)
--    x[0] = 9
-- end
