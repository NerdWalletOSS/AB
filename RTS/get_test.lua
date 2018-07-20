local assertx    = require 'lua/assertx'
local ffi        = require 'lua/ab_ffi'
local consts     = require 'lua/ab_consts'
local db_connect = require 'RTS/db_connect'
local JSON       = require 'lua/JSON'

local function mk_lkp(X)
  tmp = {}
  for _,v in pairs(X) do
    tmp[v.id] = v.name
  end
  return tmp
end

local function get_test(
  db, 
  host,
  user,
  pass,
  port,
  test_as_str,
  dbg
  )
  assert(type(db) == "string")
  assert(type(host) == "string")
  assert(type(user) == "string")
  assert(type(pass) == "string")
  assert(type(port) == "number")
  if ( type(dbg) == nil ) then 
    dbg = false
  else
    assert(type(dbg) == "boolean")
  end
  -- assert(type(test_as_str) == "number")

  local conn = db_connect(db, host, user, pass, port)
  local tmp = {}
  local states = conn:query('select id, name from state;')
  states = mk_lkp(states)

  local test_type = conn:query('select id, name from test_type;')
  test_type = mk_lkp(test_type)

  local admin = conn:query('select id, name from admin;')
  admin = mk_lkp(admin)

  local bin_type = conn:query('select id, name from bin_type;')
  bin_type = mk_lkp(bin_type)

  local fld_str = "id, name, external_id, test_type_id, state_id, bin_type_id "
  local where_str = " state_id = 3 or state_id = 4 ";

  local query_str = "select " .. fld_str .. 
                    " from test where " .. where_str .. " limit 1 "
  local test = conn:query(query_str)
  if ( not test ) then -- nothing more to do 
    return 0
  end
  assert(#test == 1 )
  local tid = tonumber(test[1].id)
  --=======================================
  where_str = "test_id = " .. tid .. " and is_del = false "
  fld_str = " name, id, custom_data, url "
  query = "select " .. fld_str .. " from variant where " .. where_str
  local variants = conn:query(query)
  test.Variants = variants
  -- get variants
  --=======================================
  conn:close()
  if ( dbg == true ) then 
    return JSON:encode(test)
  else
    ffi.copy(test_as_str, JSON:encode(test))
    return tid
  end
end
return get_test
--[[
  -- test_json.external_id = test.external_id
    -- TODO ask about channel stuff
    local query = string.format('select * from variant where test_id=%s and is_del = false;', test.id)
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
        query = string.format('select * from device_x_variant where test_id=%s and device_id=%s;', test.id, k)
        dev_arr[#dev_arr + 1] = conn:query(query)
      end
      test_json.DeviceCrossVariant = dev_arr
    end
    -- TODO here is where we send this to add test
    local str = JSON:encode(test_json)
    tests_ret[#tests_ret + 1] = str
  end
 --]]
