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
  local description = "A test to check winner percentage is set to 100"
  if ( just_pr ) then print(description) return end
  if ( not num_trials ) then num_trials = 10 end 

  for i = 1, num_trials do 
    assert(reset_db())
  
     -- Make some random test
    local tid1 = mk_rand_test()
    local T1 = get_test_info(tid1)
    local vidx = math.random(1, #(T1.Variants))
    local winner = T1.Variants[vidx].name
  
    S.publish(tid1)
    S.start(tid1)
    S.terminate(tid1, { Winner = winner})
    -- Extract the percentage of the winner variant 
    T1 = get_test_info(tid1)
    --local winner_variant_data = get_variant(test_id_1, T1.Winner)
    for k, v in pairs(T1.Variants) do
      if ( winner == v.name ) then
        assert( tonumber(v.percentage) == 100)
      else
        assert( tonumber(v.percentage) == 0)
      end
    end
    print("Test t1 succeeded")
  end
end
--===================================================
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
