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
local set_follow_on = require 'set_follow_on'

local S = require 'state_change'

local states = require 'states'
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
  -- Make test T1 and archive it 
  local T1 = dofile("good_basic1.lua")
  T1.TestType = "XYTest"
  T1.Channel = "facebook"
  local hdrs, outbody, status = mk_test(T1)
  assert(status == 200)
  local t1_id = get_test_id(hdrs)
  local T2 = get_test_info(t1_id)
  S.publish(t1_id)
  S.start(t1_id)
  S.terminate(t1_id)
  S.archive(t1_id)

  -- Make test T2 with same channel as T
  T2.name = "Test2"
  T2.id = nil
  local hdrs, outbody, status = mk_test(T2)
  assert(status == 200)

  -- Make T2 follow T1 
  local t2_id = get_test_id(hdrs)
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
  -- Make test T1 and archive it 
  local T1 = dofile("good_basic1.lua")
  T1.TestType = "XYTest"
  T1.Channel = "facebook"
  local hdrs, outbody, status = mk_test(T1)
  assert(status == 200)
  local t1_id = get_test_id(hdrs)
  local T2 = get_test_info(t1_id)
  S.publish(t1_id)
  S.start(t1_id)
  S.terminate(t1_id)
  S.archive(t1_id)

  -- Make test T2 with same channel as T
  T2.name = "Test2"
  T2.id = nil
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

