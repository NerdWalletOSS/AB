require 'lua/str'
local reset_db = require 'test_webapp/reset_db'
local get_test_info = require 'test_webapp/get_test_info'
local set_follow_on = require 'set_follow_on'
local mk_rand_test = require 'mk_rand_test'
local S = require 'state_change'
--==========================
local ssurl =  -- set state URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_set_state.php"
local tburl =  -- test basic URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_test_basic.php"
local fourl =  -- set follow on URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_set_follow_on.php"

-- STOP: Stuff common to all tests in this suite
local tests = {}
tests.t1 = function (
  just_pr
  )
  local description = "A test to confirm change in channel resets follow on"
  if ( just_pr ) then print(description) return end

   -- START: Make some test 
  local status
  reset_db()

   -- Make some random test t1 of type XYTest/Channel = facebook
  local tid1 = mk_rand_test({ TestType = "XYTest", Channel = "facebook" })
  local T1 = get_test_info(tid1)
   -- Make some random test t2 of type XYTest
  local tid2 = mk_rand_test({ TestType = "XYTest" , Channel = "facebook"})
  local T2 = get_test_info(tid2)
  -- get t2 to follow t1 --> should fail when t1 in draft mode
  status = pcall(set_follow_on, tid2, tid1); assert(not status)
  -- get t2 to follow t1 --> should fail  when t1 in dormant mode
  S.publish(tid1)
  status = pcall(set_follow_on, tid2, tid1); assert(not status)
  -- get t2 to follow t1 --> should fail  when t1 in terminated mode
  S.terminate(tid1)
  status = pcall(set_follow_on, tid2, tid1); assert(not status)
  -- get t2 to follow t1 --> should succeed  when t1 in archived mode
  S.archive(tid1)
  assert(set_follow_on(tid2, tid1))
  print("Test t1 succeeded")
  return tid1, tid2
end

tests.t2 = function()
  local tid1, tid2 = tests.t1()
  -- UTPAL TODO P1
  -- Now break channel of tid1. tid2 should not have a follow on

  tid1, tid2 = tests.t1()
  -- UTPAL TODO P1
  -- Now break channel of tid2. tid2 should not have a follow on
  print("Test t2 succeeded")
end

--===================================================
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
tests.t2() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
