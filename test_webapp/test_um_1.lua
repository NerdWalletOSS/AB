require 'lua/str'
local JSON = require 'lua/JSON'
local curl = require 'lua/curl'
local mk_test = require 'test_webapp/mk_test'
local reset_db = require 'test_webapp/reset_db'
local get_test_id = require 'test_webapp/get_test_id'
local get_test_info = require 'test_webapp/get_test_info'
local get_error_code = require 'test_webapp/get_error_code'
local states = require 'test_webapp/states'
local set_follow_on = require 'set_follow_on'
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
  local chk_T
  reset_db()
  local hdrs, outbody, status = mk_test("good_basic1.lua")

  assert(status == 200)
  local test_id_1 = get_test_id(hdrs)
  local T1 = get_test_info(test_id_1)
  assert(test_id_1 == tonumber(T1.id))

-- SET STATE of T1 to dormant
  T1.NewState = "dormant"
  T1.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T1))
 chk_T = get_test_info(test_id_1)
  assert(chk_T.State == "dormant")

-- SET STATE of T1 to started
  T1.NewState = "started"
  T1.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T1))
  assert(status == 200)
  chk_T = get_test_info(test_id_1)
  assert(chk_T.State == "started")

-- SET STATE of T1 to terminated
  T1.NewState = "terminated"
  T1.Winner = "Control"
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T1))
  assert(status == 200)
  chk_T = get_test_info(test_id_1) 
  assert(chk_T.State == "terminated")

-- SET STATE of T1 to archived
  T1.NewState = "archived"
  T1.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T1))
  assert(status == 200)
  chk_T = get_test_info(test_id_1)
  assert(chk_T.State == "archived")

  -- Create Another Test T2
  T1.name = "T2"
  T1.id = ""
  local hdrs, outbody, status = mk_test(T1)
  for k, v in pairs(hdrs) do print(k, v) end
  assert(status == 200)
  local test_id_2 = get_test_id(hdrs)
  local T2 = get_test_info(test_id_2)
  assert(test_id_2 == tonumber(T2.id))


-- T2 to follow T1
  assert(set_follow_on(test_id_2, test_id_1))


-- T2 changes channel to "aol"
  T2.Channel = "bing"
  T2.Updater =  "joe"
  local hdrs, outbody, status = curl.post(tburl, nil, JSON:encode(T2))
  assert(status == 200)
  local test_id_2 = get_test_id(hdrs)
  local T2 = get_test_info(test_id_2)
  assert(test_id_2 == tonumber(T2.id))
-- Predeccessor id of T2 to be null
  for k, v in pairs(T2) do print(k, v) end 
  assert("" == tonumber(T2.pred_id))


end



--===================================================
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
