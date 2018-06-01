require 'lua/str'
local reset_db      = require 'test_webapp/reset_db'
local mk_test       = require 'test_webapp/mk_test'
local get_test_id   = require 'test_webapp/get_test_id'
local get_test_info = require 'test_webapp/get_test_info'
local set_filters   = require 'test_webapp/set_filters'

local tests = {}
tests.t1 = function ()
  reset_db()
  local T1 = dofile("good_basic1.lua")
  local hdrs, outbody, status = mk_test(T1)
  assert(status == 200)
  local tid = get_test_id(hdrs)
  -- check filters are off and then set them all on
  local T = get_test_info(tid)
  assert(tonumber(T.has_filters) == 0 )
  for _, f in  pairs(T.CategoricalFilters) do 
    assert(tonumber(f.is_on) == 0 )
    f.is_on = "1"
  end
  T.updater_id = T.creator_id;
  local hdrs = set_filters(T)
  -- check filters are on and set them off
  local T = get_test_info(tid)
  assert(tonumber(T.has_filters) == 0 )
  for _, f in  pairs(T.CategoricalFilters) do 
    assert(tonumber(f.is_on) == 1 )
    f.is_on = "0"
  end
  local hdrs = set_filters(T)
  -- check filters are off
  local T = get_test_info(tid)
  assert(tonumber(T.has_filters) == 0 )
  for _, f in  pairs(T.CategoricalFilters) do 
    assert(tonumber(f.is_on) == 0 )
  end
  -- Set has_filters on and off and verify that it worked
  for i = 0, 1 do 
    T = get_test_info(tid)
    T.has_filters = tostring(1-i)
    local hdrs = set_filters(T)
    T = get_test_info(tid)
    assert(tonumber(T.has_filters) == 1-i )
  end
  print("Test t1 succeeded")
end
tests.t1() -- uncomment for quick and dirty testing
return tests
