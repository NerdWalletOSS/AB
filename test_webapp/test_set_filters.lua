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
  local T = get_test_info(tid)
  T.updater_id = T.creator_id;
  local hdrs = set_filters(T)
  for k, v in pairs(hdrs) do print(k, v) end
  print("Test t1 succeeded")


end
tests.t1() -- uncomment for quick and dirty testing
return tests
