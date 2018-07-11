local rand_selections = require 'test_webapp/rand_selections'
local mk_rand_test    = require 'test_webapp/mk_rand_test'
local S               = require 'test_webapp/state_change'
local reset_db        = require 'test_webapp/reset_db'

local T = {}
T.t1 = function(num_tests)
  reset_db()
  math.randomseed(os.time())
  if ( not num_tests ) then num_tests = 100 end 
  for i = 1, num_tests do 
    local tid = mk_rand_test()
    assert(S.publish(tid))
    assert(S.start(tid))
  end
  print("Successfully added " .. num_tests .. " tests in test t1")
end 
T.t1(1000) -- TO DELETE once LJT starts working
return T
