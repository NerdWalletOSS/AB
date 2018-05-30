require 'lua/str'
local reset_db      = require 'test_webapp/reset_db'
local mk_test       = require 'test_webapp/mk_test'
local get_test_id   = require 'test_webapp/get_test_id'

local S = require 'state_change'
local states = require 'states'

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
    S.publish(t1_id)
    S.start(t1_id)
    local r = math.random()
    print(i, r)
    if ( r > 0.5 ) then 
      S.terminate(t1_id)
    end
  end
end
tests.t1(10) -- uncomment for quick and dirty testing
return tests
