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
local make_follow_on = require 'make_follow_on'

local S = require 'state_change'

local states = require 'states'
--==========================

local tests = {}
tests.t1 = function (
  just_pr
  )
  local description = "TODO"
  if ( just_pr ) then print(description) return end

  -- Make test T1 and archive it 
  reset_db()
  local T1 = dofile("good_basic1.lua")
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
  assert(make_follow_on(t1_id, t2_id))

  print("Completed test t1")
end
tests.t1()
return tests

