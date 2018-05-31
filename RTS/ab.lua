local cache = require 'lua/cache'
local JSON  = require 'lua/JSON'
local x_hard_code_config = require 'lua/hard_code_config'
local x_load_config = require 'RTS/ab_load_config'
local x_update_config = require 'RTS/ab_update_config'

local tests = require 'RTS/add_test'
local reload_tests = require 'RTS/reload'

function load_config(...)
  cache.put("config", x_load_config(...))
end

function hard_code_config(...)
  cache.put("config", x_hard_code_config(...))
end

function update_config(...)
  x_update_config(...)
end
--=================================================
function preproc(...)
  tests.preproc(...)
end

function add(...)
  tests.add(...)
end


function list_tests()
  return JSON:encode(cache.get('tests'))
end

function reload(...)
  return reload_tests.reload(...)
end

function check_db_conn()
  local configs = cache.get('config').AB.DB
  local conn = load_config.db_connect(configs)
  assert(conn ~= nil, "must be a valid connection object")
end

function get_config()
  return JSON:encode(cache.get('config'))
end

function get_test_info(args)
  local j_table = JSON:decode(args)
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
  return JSON:encode(test)
end

-- function add(c_str, c_data)
--    -- print("hi from lua", c_str)
--    local x = ffi.cast("int*", c_data)
--    print(c_str)
--    x[0] = 9
-- end
