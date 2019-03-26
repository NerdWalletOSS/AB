require 'lua/str'
local mk_test       = require 'test_webapp/mk_test'
local mk_rand_test  = require 'test_webapp/mk_rand_test'
local reset_db      = require 'test_webapp/reset_db'
local get_test_id   = require 'test_webapp/get_test_id'
local get_test_info = require 'test_webapp/get_test_info'
local set_follow_on = require 'test_webapp/set_follow_on'

local S = require 'state_change'

local states = require 'states'
local function mk_and_archive(optargs)
  assert(type(optargs) == "table")
  local tid = mk_rand_test(optargs)
  local T1 = get_test_info(tid)
  if ( optargs.name )     then T1.name     = optargs.name end 
  if ( optargs.TestType ) then T1.TestType = optargs.TestType end 
  if ( optargs.Channel )  then T1.Channel  = optargs.Channel end 

  S.publish(tid)
  S.start(tid)
  S.terminate(tid)
  S.archive(tid)
  return tid
end
--==========================

local tests = {}
tests.t1 = function (
  just_pr
  )
  local description = [[
  Create test t1 of type XYTest and Channel X and then archive it. 
  Create test t2 with same type and Channel. 
  While t2 is in draft mode, make it follow t1. Should work.
    ]]
  if ( just_pr ) then print(description) return end

  reset_db()
  local X = {}
  X.name = "T1"; X.TestType = "XYTest"; X.Channel = "facebook"
  local t1_id = assert(mk_and_archive(X))

  -- Make test T2 with same channel as T
  X = {}
  X.name = "T2"; X.TestType = "XYTest"; X.Channel = "facebook"
  local t2_id = mk_rand_test(X)
  assert(set_follow_on(t2_id, t1_id))

  print("Completed test t1")
end
tests.t2 = function (
  just_pr
  )
  local description = [[
  Create test t1 of type XYTest and Channel X and then archive it. 
  Create test t2 with same type and Channel. 
  Change state of t2 to other than draft and make it follow t1. 
  Should NOT work.
    ]]
  if ( just_pr ) then print(description) return end

  reset_db()
  local X = {}; X.TestType = "XYTest"; X.Channel = "facebook"
  local t1_id = assert(mk_and_archive(X))

  -- Make test T2 with same channel as T
  local T2 = get_test_info(t1_id)
  T2.name = "Test2"
  T2.id = nil
  T2.State = nil
  local hdrs, outbody, status = mk_test(T2)
  assert(status == 200)
  local t2_id = get_test_id(hdrs)

  -- Publish and attempt to follow. Must fail.
  S.publish(t2_id)
  status = pcall(set_follow_on, t2_id, t1_id)
  assert(not status)
  -- Start and attempt to follow. Must fail.
  S.start(t2_id)
  status = pcall(set_follow_on, t2_id, t1_id)
  assert(not status)
  -- Terminate and attempt to follow. Must fail.
  S.terminate(t2_id)
  status = pcall(set_follow_on, t2_id, t1_id)
  assert(not status)
  -- Archive and attempt to follow. Must fail.
  S.archive(t2_id)
  status = pcall(set_follow_on, t2_id, t1_id)
  assert(not status)


  print("Completed test t2")
end
tests.t1()
tests.t2()
return tests

