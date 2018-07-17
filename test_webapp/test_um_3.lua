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
  just_pr
  )
  local description = "A test to check possibilities of deleting/archiving the test case"
  if ( just_pr ) then print(description) return end

   -- START: Reset DB
  assert(reset_db())

   -- Make some random test
  local test_types = { "ABTest", "XYTest" }
  math.randomseed(os.time())
  local idx = math.random(#test_types)
  local test_type = test_types[idx]
  local optargs = {}
  optargs.TestType = test_type
  local tid1 = mk_rand_test(optargs)
  local T1 = get_test_info(tid1)

  -- archived/delete the test
  T1.NewState = "archived"
  T1.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T1))
  assert(status == 200)
  local chk_T
  chk_T = get_test_info(tid1)
  -- for k, v in pairs(chk_T) do print(k, v) end 
  assert(chk_T.State == "archived")

  -- Reproduce test 1 of same testtype XYTest
  local optargs = {}
  optargs.TestType = test_type
  local tid2 = mk_rand_test(optargs)
  local T2 = get_test_info(tid2)

  -- Publish
  S.publish(tid2)

  -- archived/delete the test
  T2.NewState = "archived"
  T2.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T2))
  assert(status ~= 200)

end

--===================================================
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
