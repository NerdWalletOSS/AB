
local S = require 'test_webapp/state_changes'
local mk_rand_test = require 'test_webapp/mk_rand_test'


local T = {}
T.t1 = function(num_iters)
  if ( not num_iters ) then num_iters = 100 end 
  for i = 1, num_iters do
    local T1, T2, T3
    -- make a test 
    local hdrs, body, status = mk_rand_test()
    local tid = get_test_id(hdrs)
    --= change percentages. it should work 
    T1 = get_test_info(tid)
    T2, status = set_random_percs(T1)
    assert(status)
    T3 = get_test_info(tid)
    assert(compare_test_info(T2, T3))
    -- publish it  and change percentages. it should work.
    S.publish(tid)
    T2, status = set_random_percs(T1)
    assert(status)
    T3 = get_test_info(tid)
    assert(compare_test_info(T2, T3))
    -- start it  and change percentages. it should work.
    S.start(tid)
    T2, status = set_random_percs(T1)
    assert(status)
    T3 = get_test_info(tid)
    assert(compare_test_info(T2, T3))
    -- terminate test and try to change percentages. should not work 
    S.terminate(tid)
    T2, status = set_random_percs(T1)
    assert(not status)
    -- archive test and try to change percentages. should not work 
    S.archive(tid)
    T2, status = set_random_percs(T1)
    assert(not status)
  end
end
T.t1(100)
return T
