local assertx    = require 'lua/assertx'
local ffi        = require 'lua/ab_ffi'
local consts     = require 'lua/ab_consts'
local db_connect = require 'RTS/db_connect'
local JSON       = require 'lua/JSON'

local function get_test(
  db, 
  host,
  user,
  pass,
  port,
  tid,
  exact -- true => you want this tid, false => you want greater than tid
  )
  assert(type(aux) == "table", "Global aux table missing")
  assert(type(db) == "string")
  assert(type(host) == "string")
  assert(type(user) == "string")
  assert(type(pass) == "string")
  assert(type(port) == "number")
  assert(type(tid)  == "number")
  if ( not exact ) then exact = false end 
  assert(type(exact) == "boolean")

  local conn = db_connect(db, host, user, pass, port)

  local fld_str = "id, name, seed, has_filters, ramp, external_id, test_type_id, state_id, is_dev_specific, bin_type_id "
  local where_str 
  if ( exact ) then 
    where_str = " id = " .. tid;
  else
    where_str = " ( state_id = 3 or state_id = 4 ) and id > " .. tid;
  end 

  local query_str = "select " .. fld_str .. 
                    " from test where " .. where_str .. " limit 1 "
  local test = conn:query(query_str)
    -- Check if nothing more to do 
  if ( not test    ) then conn:close() return 0 end
  if ( not test[1] ) then conn:close() return 0 end
  test = test[1]
  local tid = tonumber(test.id)
  local state_id = tonumber(test.state_id)
  test.State    = assert(aux.states[test.state_id])
  local test_type_id = tonumber(test.test_type_id)
  test.TestType = assert(aux.test_type[test.test_type_id])
  -- Following is temp hack to enable simpler JSON parsing in C
  test.id       = tostring(test.id)
  test.external_id       = tostring(test.external_id)
  test.is_dev_specific       = tostring(test.is_dev_specific)
  --=======================================
  -- get variants
  where_str = "test_id = " .. tid .. " and is_del = false "
  fld_str = " name, id, is_final, percentage, custom_data, url "
  query_str = "select " .. fld_str .. " from variant where " .. where_str
  local variants = conn:query(query_str)
  test.Variants = variants
  test.NumVariants = tostring(#variants)
  --=======================================
  --select percentage, device_id, variant_id from device_x_variant 
  --= 5;
  if ( test.TestType == "XYTest" ) then 
    test.DeviceCrossVariant = {}
    for device_id, device_name in pairs(aux.device) do 
      where_str = "test_id = " .. tid .. " and device_id = " .. device_id
      fld_str = " percentage, device_id, variant_id "
      query_str = "select " .. fld_str .. " from device_x_variant where " .. where_str
      test.DeviceCrossVariant[device_name] = conn:query(query_str)
    end
  end
  --=======================================
  conn:close()
  collectgarbage()
  return tid, JSON:encode(test)
end
return get_test
