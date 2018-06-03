require 'lua/str'
local JSON = require 'lua/JSON'
local curl = require 'lua/curl'
local mk_test = require 'test_webapp/mk_test'
local reset_db = require 'test_webapp/reset_db'
local get_test_id = require 'test_webapp/get_test_id'
local get_test_info = require 'test_webapp/get_test_info'
local get_error_code = require 'test_webapp/get_error_code'
local states = require 'test_webapp/states'
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
  local description = "A test to check winner percentage is set to 100"
  if ( just_pr ) then print(description) return end

   -- START: Make some test 
  local chk_T
  reset_db()
  local hdrs, outbody, status = mk_test("good_basic1.lua")
  assert(status == 200)
  local test_id_1 = get_test_id(hdrs)
  local T1 = get_test_info(test_id_1)
  assert(test_id_1 == tonumber(T1.id))
  -- for k, v in pairs(T1) do print(k, v) end 

  -- Publish
  S.publish(test_id_1)
  -- Start
  S.start(test_id_1)
  -- Terminate
  local V = T1.Variants
  local vidx = math.random(#V)
  T.Winner = V[vidx].name
  S.terminate(test_id_1,T.Winner)
  local T1 = get_test_info(test_id_1)
  assert(test_id_1 == tonumber(T1.id))
  -- Extract the percentage of the winner variant
  



  print("Test t1 succeeded")
end



--===================================================
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
