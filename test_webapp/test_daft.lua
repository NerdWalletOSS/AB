require 'lua/str'
local reset_db      = require 'test_webapp/reset_db'
local mk_test       = require 'test_webapp/mk_test'
local get_test_id   = require 'test_webapp/get_test_id'
local get_tests     = require 'test_webapp/get_tests'
local S             = require 'test_webapp/state_change'
local states        = require 'test_webapp/states'
local compare_T     = require 'test_rts/compare_T'

local tic_url = "localhost:8000/TestInfo?Source=C"
local tbp_url = "localhost:8080/AB/php/endpoints/endpoint_test_basic.php"

local function advance_state(tid)
  local r = math.random()
  if ( r < 0.8 ) then 
    S.publish(tid)
  end
  if ( r < 0.4 ) then 
    S.start(tid)
  end
  if ( r < 0.2 ) then 
    S.terminate(tid)
  end
  if ( r < 0.1 ) then 
    S.archive(tid)
  end
end

local tests = {}
tests.t1 = function(num_tests)
  reset_db()
  if not num_tests then num_tests = 10 end 
  assert(type(num_tests) == "number")
  math.randomseed( os.time() )
  local T1 = dofile("good_basic1.lua")
  for i = 1, num_tests do 
    T1.name = "Test_" .. i 
    local hdrs, outbody, status = mk_test(T1)
    assert(status == 200)
    local t1_id = get_test_id(hdrs)
    advance_state(t1_id)
  end
  print("Test t1 succeeded")
end
-- tests.t1(10) -- uncomment for quick and dirty testing
tests.t2 = function(
  num_tests, 
  num_iters,
  chk_rts
  )
  if not num_tests then num_tests = 10 end 
  if not num_iters then num_iters = 1  end 
  if not chk_rts   then chk_rts = false end
  tests.t1(num_tests)
  local Ts = get_tests()
  -- some random state changes. We use pcall because not all these
  -- transitions are legitimate
  for i = 1, num_iters do
    for test_id, _ in pairs(Ts) do
      local status = pcall(advance_state, test_id)
      local T = get_test_info(test_id)
      local state = T.State
      local TestType = T.TestType
      local test_name = T.name
      if ( ( state == "started" ) and ( state == "terminated" )  and
        ( chk_rts ) ) then
      end
      local url = tic_url .. "&TestType=" .. TestType .. "&Name=" .. name
      a, b, c = curl.get(tic_url)
      local chk_T = JSON:decode(b)
      assert(compare_T(T,  chk_T))
    end
  end
  print("Test t2 succeeded")
end
-- tests.t2(100, 100, false) -- uncomment for quick and dirty testing
return tests
