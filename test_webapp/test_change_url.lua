require 'lua/str'
local JSON = require 'lua/JSON'
local curl = require 'lua/curl'
local mk_test = require 'test_webapp/mk_test'
local mk_rand_test = require 'test_webapp/mk_rand_test'
local reset_db = require 'test_webapp/reset_db'
local get_test_id = require 'test_webapp/get_test_id'
local get_test_info = require 'test_webapp/get_test_info'
local get_error_code = require 'test_webapp/get_error_code'
local states         = require 'test_webapp/states'
local S              = require 'test_webapp/state_change'
--==========================
local ssurl =  -- set state URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_set_state.php"
local tburl =  -- test basic URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_test_basic.php"

local tests = {}
tests.t1 = function (
  just_pr
  )
  local description = "Testing over-writing change of URL in started"
  if ( just_pr ) then print(description) return end

  -- START: Make some test 
  reset_db()
  local tid = assert(mk_rand_test({TestType = "XYTest"}))
  local T1 = get_test_info(tid)
  S.publish(tid)
  S.start(tid)
  --=================================
  local T2 = {}
  T2.id           = T1.id
  T2.name         = T1.name
  T2.State        = T1.State
  T2.Updater      = T1.Creator
  T2.TestType     = T1.TestType
  T2.BinType      = T1.BinType 
  T2.OverWriteURL = "true"
  T2.Variants = T1.Variants
  local new_url = "http://www.nerdwallet.com"
  local old_url  = T2.Variants[1].url
  local old_name = T2.Variants[1].name 
  local old_id   = T2.Variants[1].id 
  T2.Variants[1].url = new_url
  local hdrs, outbody, status = curl.post(tburl, nil, JSON:encode(T2))
  -- for k, v in pairs(hdrs) do print(k, v) end 
  assert(status == 200)
  local T3 = get_test_info(tid)
  for k, v in pairs(T3.Variants) do 
    if ( v.name == old_name ) then
      assert(v.url == new_url)
      assert(v.id  ~= old_id)
    end
  end
  print("Completed test t1")
end
--===========================
tests.t2 = function (
  just_pr
  )
  local description = [[
  When state == started and OverWrite = false, then no change in URL possible
  ]]
  if ( just_pr ) then print(description) return end
  print("TODO UTPAL t2")
end
--===========================
tests.t3 = function (
  just_pr
  )
  local description = [[
  When state == started and OverWrite = true and new URLs are same as old
  then no new rows created in Variant table
  ]]
  if ( just_pr ) then print(description) return end
  print("TODO UTPAL t3")
end
--===========================
tests.t4 = function (
  just_pr
  )
  local description = [[
  When state != started and OverWrite = true, 
  then no new rows created in Variant table
  ]]
  if ( just_pr ) then print(description) return end
  print("TODO UTPAL t4")
end

tests.t1()
return tests
