require 'lua/str'
local JSON      = require 'lua/JSON'
local curl      = require 'lua/curl'
local mk_test   = require 'test_webapp/mk_test'
local reset_db  = require 'test_webapp/reset_db'
local mk_rand_test   = require 'test_webapp/mk_rand_test'
local get_test_info  = require 'test_webapp/get_test_info'
local get_error_code = require 'test_webapp/get_error_code'
local states = require 'test_webapp/states'
local S      = require 'test_webapp/state_change'
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
  local winners = {}
  local optargs = {}
  optargs.TestType = "XYTest"
  local tid1 = mk_rand_test(optargs)
  local T1 = get_test_info(tid1)
  winners["XYTest"] = T1.Variants[1].name

  -- Create test of type = ABTest
  local tid2 = mk_rand_test({ TestType = "ABTest"})
  local T2 = get_test_info(tid2)
  winners.ABTest = T2.Variants[1].name

  local tids = { }
  tids.XYTest = tid1
  tids.ABTest = tid2
  for k, tid in pairs(tids)  do
    S.publish(tid)
    S.start(tid)
    S.terminate(tid, { Winner = winners[k] })
  end
  for k, tid in pairs(tids)  do
    local status = pcall(S.start, tid)
    if ( k == "XYTest" ) then assert(status) end  -- resurrect XYTest
    if ( k == "ABTest" ) then assert(not status) end -- not resurrect ABTest
  end
  print("Test t1 succeeded")
end
--===================================================
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
