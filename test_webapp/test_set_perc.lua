
local S = require 'test_webapp/state_change'
local mk_rand_test = require 'test_webapp/mk_rand_test'
local get_test_info = require 'test_webapp/get_test_info'
local set_perc = require 'test_webapp/set_perc'
local reset_db = require 'test_webapp/reset_db'


local T = {}
T.t1 = function(num_iters)
  reset_db()
  if ( not num_iters ) then num_iters = 100 end 
  for i = 1, num_iters do
    local T1, T2, T3, P, status
    -- make a test 
    local tid = mk_rand_test()
    T1 = get_test_info(tid)
    --= change percentages. it should work 
    P  = set_perc(tid)
    T3 = get_test_info(tid)
    -- assert(compare_test_info(T2, T3))
    -- publish it  and change percentages. it should work.
    S.publish(tid)
    P  = set_perc(tid)
    T3 = get_test_info(tid)
    -- assert(compare_test_info(T2, T3))
    -- start it  and change percentages. it should work.
    S.start(tid)
    P  = set_perc(tid)
    T3 = get_test_info(tid)
    -- assert(compare_test_info(T2, T3))
    -- terminate test and try to change percentages. should not work 
    S.terminate(tid)
    status = pcall(set_perc, tid)
    assert(not status)
    -- archive test and try to change percentages. should not work 
    S.archive(tid)
    status = pcall(set_perc, tid)
  end
end
T.t1(100)
return T
