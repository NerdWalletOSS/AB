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
local db_count       = require 'test_webapp/db_count'
--==========================
local ssurl =  -- set state URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_set_state.php"
local tburl =  -- test basic URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_test_basic.php"

local function mk_input_for_update(T1)
  local T2 = {}
  T2.id           = T1.id
  T2.name         = T1.name
  T2.State        = T1.State
  T2.Updater      = T1.Creator
  T2.TestType     = T1.TestType
  T2.BinType      = T1.BinType 
  T2.Variants = T1.Variants
  return T2
end

local tests = {}

tests.t1 = function (
  just_pr
  )
  local description = [[
  When test is started and OverWriteURL is "true", then URL can be changed
  Also, a new ID is assigned to variant with new URL
  Also, a new ID is NOT assigned to variant with same URL as before
  ]]
  if ( just_pr ) then print(description) return end

  -- START: Make some test 
  reset_db()
  local tid = assert(mk_rand_test({TestType = "XYTest"}))
  local T1 = get_test_info(tid)
  S.publish(tid)
  S.start(tid)
  T1 = get_test_info(tid)
  assert(T1.State == "started", T1.State)
  local nV = #T1.Variants
  -- print("NumVariants = ", nV)
  --=================================
  local T2 = mk_input_for_update(T1)
  T2.OverWriteURL = "true"
  local new_url = "http://www.nerdwallet.com"
  local vidx = 1  --- index of variant to change 
  T2.Variants[vidx].url = new_url
  local old_name = T2.Variants[vidx].name
  local hdrs, outbody, status = curl.post(tburl, nil, JSON:encode(T2))
  -- for k, v in pairs(hdrs) do print(k, v) end 
  assert(status == 200)
  local T3 = get_test_info(tid)
  for k, v in pairs(T3.Variants) do 
    if ( v.name == old_name ) then 
      assert(v.url == new_url, v.url .. " <-->  " .. new_url )
      assert(v.id ~= T2.Variants[k].id)
    else -- no change to anybody else
      for k2, v2 in pairs(T2.Variants) do
        if ( v.name == v2.name ) then 
          assert(v.url == v2.url)
          assert(v.id  == v2.id)
        end
      end
    end
  end
  assert(nV+1 == db_count("variant"))
  print("Completed test t1")
end

--===========================
tests.t2 = function (
  just_pr
  )
  local description = [[
  When test is started and OverWriteURL is "false", 
  then URL cannot be changed
  ]]
  if ( just_pr ) then print(description) return end
 -- START: Make some test 
  reset_db()
  local tid = assert(mk_rand_test({TestType = "XYTest"}))
  local T1 = get_test_info(tid)
  S.publish(tid)
  S.start(tid)
  local T2 = mk_input_for_update(T1)
  T2.OverWriteURL = "false"
  local vidx = 1 -- index of variant whose URL is to be changed
  local new_url = "http://www.nerdwallet.com"
  T2.Variants[vidx].url = new_url
  local hdrs, outbody, status = curl.post(tburl, nil, JSON:encode(T2))
  assert(status == 200)
  local T3 = get_test_info(tid)
  for k, v in pairs(T3.Variants) do 
    assert(v.url == T2.Variants[k].url)
    assert(v.id  == T2.Variants[k].id)
  end
  print("completed test t2")
end
--===========================
tests.t3 = function (
  just_pr
  )
  local description = [[
  When state == started and OverWrite = true and 
   new URLs are same as old then 
   no new rows created in Variant table ]]
  if ( just_pr ) then print(description) return end
 -- START: Make some test 
  reset_db()
  local tid = assert(mk_rand_test({TestType = "XYTest"}))
  local T1 = get_test_info(tid)
  S.publish(tid)
  S.start(tid)
  --=================================
  local T2 = mk_input_for_update(T1)
  T2.OverWriteURL = "true"
  local hdrs, outbody, status = curl.post(tburl, nil, JSON:encode(T2))
  assert(status == 200)
  local T3 = get_test_info(tid)
  for k, v in pairs(T3.Variants) do 
    assert(v.url == T2.Variants[k].url)
    assert(v.id  == T2.Variants[k].id)
  end
  print("completed test t3")
end
--===========================
tests.t4 = function (
  just_pr
  )
  local description = [[
  When state != started OverWriteURL is ignored. 
  Can change URL without new rows going into variant table
  ]]
  -- START: Make some test 
  reset_db()
  local tid = assert(mk_rand_test({TestType = "XYTest"}))
  for i = 1, 2 do 
    local new_url 
    local T1 = get_test_info(tid)
    if ( i == 1 ) then 
      new_url = "http://www.nerdwallet.com"
    elseif ( i == 2 ) then 
      new_url = "http://www.google.com"
      S.publish(tid) 
    else 
      assert(nil)
    end 
    local T2 = mk_input_for_update(T1)
    T2.OverWriteURL = "false"
    local vidx = math.random(1, #T2.Variants)
    T2.Variants[vidx].url = new_url
    local hdrs, outbody, status = curl.post(tburl, nil, JSON:encode(T2))
    -- for k, v in pairs(hdrs) do print(k, v) end 
    assert(status == 200)
    local T3 = get_test_info(tid)
    for k, v in pairs(T3.Variants) do 
      if ( k == vidx ) then 
        assert(v.url == new_url)
        assert(v.id  == T2.Variants[k].id)
      else 
        assert(v.url == T2.Variants[k].url)
        assert(v.id  == T2.Variants[k].id)
      end
    end
  end
  if ( just_pr ) then print(description) return end
  print("completed test t4")
end
-- tests.t4()
-- tests.t2()
-- tests.t3()
tests.t1()
return tests
