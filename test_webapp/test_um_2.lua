require 'lua/str'
local JSON = require 'lua/JSON'
local curl = require 'lua/curl'
local mk_test = require 'test_webapp/mk_test'
local reset_db = require 'test_webapp/reset_db'
local mk_rand_test = require 'test_webapp/mk_rand_test'
local get_test_id = require 'test_webapp/get_test_id'
local get_test_info = require 'test_webapp/get_test_info'
local get_error_code = require 'test_webapp/get_error_code'
local S = require 'state_change'
local states = require 'states'
--==========================
local ssurl =  -- set state URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_set_state.php"
local tburl =  -- test basic URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_test_basic.php"

-- STOP: Stuff common to all tests in this suite
local tests = {}
tests.t1 = function (
  just_pr,
  num_trials
  )
  local description = "A test to check possibilities of duplicate name of test"
  if ( just_pr ) then print(description) return end
  if ( not num_trials ) then num_trials = 10 end 

  for i = 1, num_trials do 
    assert(reset_db())
    local status
    local newTestType
     -- Make some random test
    local tid1 = mk_rand_test()
    local T1 = get_test_info(tid1)
    local name = T1.name
    local TestType = T1.TestType
    -- Try to create a test with same name again. Should fail
    status = pcall(mk_rand_test, { name = name, TestType = TestType})
    assert( not status)
    -- Creating test with same name different type should work 
    if ( TestType == "ABTest" ) then 
      newTestType = "XYTest"
    elseif ( TestType == "XYTest" ) then 
      newTestType = "ABTest"
    else
      assert(nil)
    end
    status = pcall(mk_rand_test, { name = name, TestType = newTestType})
    assert(status)
  
    S.publish(tid1)
    S.start(tid1)
    S.terminate(tid1)
    S.archive(tid1)
    -- Try to create a test with same name again. Should work
    status = pcall(mk_rand_test, { name = name, TestType = TestType})
    assert( status)
  end
  print("Test t1 succeeded")
end
--===================================================
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
