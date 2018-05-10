package.path = package.path .. ";./../src/?.lua;./../test_webapp/?.lua;../test_generator/?.lua"
require 'str'
local JSON = require 'JSON'
local plfile = require 'pl.file'
local plpath = require 'pl.path'
local curl = require 'curl'
local mk_test = require 'mk_test'
local reset_db = require 'reset_db'
local get_test_id = require 'get_test_id'
local get_test_info = require 'get_test_info'
local get_error_code = require 'get_error_code'
local states = require 'states'
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

  for k, v in pairs(T1) do print(k, v) end 

  -- Create Another Test T2
  T1.name = "T2"
  T1.id = ""
  local hdrs, outbody, status = mk_test(T1)
  for k, v in pairs(hdrs) do print(k, v) end
  assert(status == 200)
  local test_id_2 = get_test_id(hdrs)
  local T2 = get_test_info(test_id_2)
  assert(test_id_2 == tonumber(T2.id))

-- SET STATE of T2 to dormant
  T2.NewState = "dormant"
  T2.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T2))
  assert(status == 200)
  chk_T = get_test_info(test_id_2)

  assert(chk_T.State == "dormant")

-- SET STATE of T2 to started
  T2.NewState = "started"
  T2.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T2))
  assert(status == 200)
  chk_T = get_test_info(test_id_2)
  -- for k, v in pairs(chk_T) do print(k, v) end 
  assert(chk_T.State == "started")

-- SET STATE of T2 to terminated
  T2.NewState = "terminated"
  T2.Winner = "Control"
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T2))
  assert(status == 200)
  chk_T = get_test_info(test_id_2)
  -- for k, v in pairs(chk_T) do print(k, v) end 
  assert(chk_T.State == "terminated")

-- SET STATE of T2 to archived
  T2.NewState = "archived"
  T2.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T2))
  assert(status == 200)
  chk_T = get_test_info(test_id_2)
  -- for k, v in pairs(chk_T) do print(k, v) end 
  assert(chk_T.State == "archived")


-- T1 to follow T2
-- TODO: HOW TO SET FOLLOW ON PROPERLY. TO TAKE GUIDANCE FROM RAMESH SIR
  local T3 = test_id_1, test_id_2
  local hdrs, outbody, status = curl.post(fourl, nil, JSON:encode(T3))
  assert(status == 200)


-- T1 changes channel to "aol"
  T1.Channel = "facebook"
  local hdrs, outbody, status = curl.post(tburl, nil, JSON:encode(T1))
  for k, v in pairs(hdrs) do print(k, v) end
  assert(status == 200)
  local test_id_1 = get_test_id(hdrs)
  local T1 = get_test_info(test_id_1)
  assert(test_id_1 == tonumber(T1.id))
-- Predeccessor id of T1 to be null
-- how to check pred_id?
  assert("" == tonumber(T1.pred_id))


end



--===================================================
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
