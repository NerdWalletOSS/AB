require 'lua/str'
local JSON = require 'lua/JSON'
local curl = require 'lua/curl'
local mk_test = require 'test_webapp/mk_test'
local reset_db = require 'test_webapp/reset_db'
local get_test_id = require 'test_webapp/get_test_id'
local get_test_info = require 'test_webapp/get_test_info'
local get_error_code = require 'test_webapp/get_error_code'
local states = require 'test_webapp/states'
local S = require 'test_webapp/state_change'
--==========================
local ssurl =  -- set state URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_set_state.php"
local tburl =  -- test basic URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_test_basic.php"

-- STOP: Stuff common to all tests in this suite
local tests = {}
tests.t1 = function (
  just_pr
  )
  local description = [[
  A test to check that terminated test can only be started in 
  case of XYTest test type]]
  if ( just_pr ) then print(description) return end

  assert(reset_db())
   -- START: Make some test of type XYTest
  local hdrs, outbody, status = mk_test("good_basic4.lua")
  assert(status == 200)
  local tid1 = get_test_id(hdrs)
  local T1 = get_test_info(tid1)

  -- Create test name with same name but type = ABTest
  T1.TestType = "ABTest"
  T1.id = ""
  local hdrs, outbody, status = mk_test(T1)
  assert(status == 200)
  local tid2 = get_test_id(hdrs)

  local tids = { }
  tids.XYTest = tid1
  tids.ABTest = tid2
  local optargs = { }
  optargs.Winner = "Control"
  for k, tid in pairs(tids)  do
    S.publish(tid)
    S.start(tid)
    S.terminate(tid, optargs)
  end
  for k, tid in pairs(tids)  do
    status = pcall(S.start, tid)
    if ( k == "XYTest" ) then assert(status) end  -- resurrect XYTest
    if ( k == "ABTest" ) then assert(not status) end -- not resurrect ABTest
  end
  print("Test t1 succeeded")
end
--===================================================
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
