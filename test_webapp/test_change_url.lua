require 'lua/str'
local JSON = require 'lua/JSON'
local curl = require 'lua/curl'
local mk_test = require 'test_webapp/mk_test'
local reset_db = require 'test_webapp/reset_db'
local get_test_id = require 'test_webapp/get_test_id'
local get_test_info = require 'test_webapp/get_test_info'
local get_error_code = require 'test_webapp/get_error_code'
local states = require 'test_webapp/states'
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
  local chk_T
  reset_db()
  local hdrs, outbody, status = mk_test("good_basic1.lua")
  assert(status == 200)
  local test_id = get_test_id(hdrs)
  local T = get_test_info(test_id)
  assert(test_id == tonumber(T.id))

  -- T.name = ""
  -- T.id = nil
  -- local hdrs, outbody, status = mk_test(T)
  -- assert(status ~= 200)
  -- Move state to dormant 
  T.NewState = "dormant"
  T.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  chk_T = get_test_info(test_id)
  -- for k, v in pairs(chk_T) do print(k, v) end 
  assert(chk_T.State == "dormant")
  --=================================
  -- Move state to started 
  T.NewState = "started"
  T.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  T = get_test_info(test_id)
  assert(T.State == "started")
  T.Updater =  "joe" -- TODO Improve this hard coding
  --=================================
  T.OverWriteURL = "true"
  local new_url = "http://www.nerdwallet.com"
  local old_url  = T.Variants[1].url
  local old_name = T.Variants[1].name 
  local old_id   = T.Variants[1].id 
  T.Variants[1].url = new_url
  local hdrs, outbody, status = curl.post(tburl, nil, JSON:encode(T))
  -- for k, v in pairs(hdrs) do print(k, v) end 
  assert(status == 200)
  T = get_test_info(test_id)
  for k, v in pairs(T.Variants) do 
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
