local JSON          = require 'lua/JSON'
local get_test_info = require 'test_webapp/get_test_info'
local mk_rand_test  = require 'test_webapp/mk_rand_test'
local S             = require 'test_webapp/state_change'
local R             = require 'test_webapp/rand_selections'
local reset_db      = require 'test_webapp/reset_db'
local set_dev_specific      = require 'test_webapp/set_dev_specific'
local mk_input_for_dev_specific_test = 
  require 'test_webapp/mk_input_for_dev_specific_test'


local function mk_dev_specific_test()
  local tid = mk_rand_test({ TestType = "XYTest"})
  local T = get_test_info(tid)
  S.publish(tid)
  T = get_test_info(tid)
  local D = require '../test_webapp/devices'
  local V = T.Variants
  local nV = T.NumVariants
  local out = mk_input_for_dev_specific_test(tid, D, V, nV)
  out.is_dev_specific = true
  local hdrs, body, status = set_dev_specific(out)
  S.start(tid)
  return tid
end
return mk_dev_specific_test
-- local tid = mk_dev_specific_test() print(tid)
