require 'lua/str'
local JSON = require "lua/JSON"
local curl = require "lua/curl"
local get_test_id = require 'test_webapp/get_test_id'
local compare_tests = require 'test_webapp/compare_tests'
local get_test_info = require 'test_webapp/get_test_info'
local mk_rand_test = require 'test_webapp/mk_rand_test'
local reset_db = require 'test_webapp/reset_db'
local get_db_config = require 'test_webapp/get_db_config'
local S             = require 'test_webapp/state_change'

require 'RTS/ab'
local get_test      = require 'RTS/get_test'
local load_aux      = require 'RTS/load_aux'

local suite_description = [[
  These tests are about setting description and custom data
]]

local url =  -- test basic URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_add_addnl_var_info.php"

local tests = {}
tests.t1 = function (
  num_iters,
  just_pr
  )
  local description = "Create a number of valid tests "
  if ( just_pr ) then print(description) return end
  if ( not num_iters ) then num_iters = 100 end 

  local choices = {}
  choices[1] = { 
    description = " some bogus description ",
    custom_data = ' { \"some\" : \"bogus\" } ',
  }
  choices[2] = { 
    description = " ",
    custom_data = ' ',
  }
  reset_db()
  local db, host, user, pass, port = get_db_config()
  load_aux(db, host, user, pass, port)
  for i = 1, num_iters do 
    local tid = mk_rand_test()
    local T1 = get_test_info(tid)
    -- select arbitrary variant_id to modify
    local Variants = T1.Variants
    local vidx = math.random(1, #Variants)
    local variant_id = T1.Variants[vidx]['id']
    for k, v in pairs(choices) do 
  
      local description = v.description
      local custom_data = v.custom_data
      Variants[vidx]['custom_data'] = custom_data
      Variants[vidx]['description'] = description
  
      local T2 = {}
      T2.id = tid
      T2.Updater = T1.Creator
      T2.VariantID = variant_id
      T2.Variants = Variants
      local hdrs, outbody, status = curl.post(url, nil, JSON:encode(T2))
      -- for k, v in pairs(hdrs) do print(k, v) end 
      assert(status == 200)
      local tid3, T3 = get_test(db, host, user, pass, port, tid, true)
      T3 = JSON:decode(T3)
      for k, v in pairs(T3.Variants) do 
        if ( v.id == variant_id ) then 
          assert(v.description == description) 
          assert(v.custom_data == custom_data) 
        end
      end
    end
    local T4 = {}
    T4.id = tid
    T4.Updater = T1.Creator
    T4.VariantID = variant_id
    -- send bad custom data 
    Variants[vidx]['custom_data'] = " some bogus custom data "
    T4.Variants = Variants
    local hdrs, outbody, status = curl.post(url, nil, JSON:encode(T4))
    assert(status ~= 200)
    -- try and make changes after termination. Should fail
    S.publish(tid)
    S.start(tid)
    S.terminate(tid)
    Variants[vidx]['custom_data'] = ' { "good" :  "custom data" } '
    Variants[vidx]['description'] = " description "
    T4.Variants = Variants
    hdrs, outbody, status = curl.post(url, nil, JSON:encode(T4))
    assert(status ~= 200)
    -- try and make changes after archive. Should fail
    S.terminate(tid)
    hdrs, outbody, status = curl.post(url, nil, JSON:encode(T4))
    assert(status ~= 200)
  end
  print("Test t1 succeeded")
  return true
end
tests.t1() 
return tests, suite_description
