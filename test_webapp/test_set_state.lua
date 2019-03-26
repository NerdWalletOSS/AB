require 'lua/str'
local JSON = require 'lua/JSON'
local curl = require 'lua/curl'
local mk_test = require 'test_webapp/mk_test'
local reset_db = require 'test_webapp/reset_db'
local get_test_id = require 'test_webapp/get_test_id'
local get_test_info = require 'test_webapp/get_test_info'
local get_error_code = require 'test_webapp/get_error_code'
local states       = require 'test_webapp/states'
local S            = require 'test_webapp/state_change'
local mk_rand_test = require 'test_webapp/mk_rand_test'
local get_db_config = require 'test_webapp/get_db_config'

require 'RTS/ab'
local get_test      = require 'RTS/get_test'
local load_aux      = require 'RTS/load_aux'
--==========================
local ssurl =  -- set state URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_set_state.php"


local tests = {}
tests.t1 = function (
  just_pr
  )
  local description = "Testing set_state when draft"
  if ( just_pr ) then print(description) return end

  local db, host, user, pass, port = get_db_config()
  load_aux(db, host, user, pass, port)

  -- START: Make some test 
  reset_db()
  local tid = assert(mk_rand_test())
  local T = get_test_info(tid)
  assert(T.State == "draft")
  local tid2, T2 = get_test(db, host, user, pass, port, tid, true)
  T2 = JSON:decode(T2)
  assert(T2.State == "draft")
  --======== now do the bad things, in any order
  local bad_states = { "started", "terminated" }
  for k, bad_state in pairs(bad_states) do 
    T.NewState = bad_state
    local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
    local error_code = get_error_code(hdrs)
    assert(error_code == 400)
  end
  --========= select a random good thing t do 
  local good_states = { "draft", "dormant",  "archived" }
  math.randomseed(os.time())
  local idx = math.random(#good_states)
  local good_state = good_states[idx]
  print("moving from draft to " .. good_state)
  T.NewState = good_state
  T.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  local Tchk = get_test_info(tid)
  --====================================
  print("Test t1 succeeded")
  return true
end
tests.t2 = function (
  just_pr
  )
  local description = "Testing set_state when dormant"
  if ( just_pr ) then print(description) return end

  -- START: Make some test 
  reset_db()
  local hdrs, outbody, status = mk_test("good_basic1.lua")
  assert(status == 200)
  local tid = get_test_id(hdrs)
  local T = get_test_info(tid)
  local tid = tonumber(T.id)
  -- Move state to dormant 
  T.NewState = "dormant"
  T.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  --======== now do the bad things, in any order
  local bad_states = { "draft", "terminated" }
  for k, bad_state in pairs(bad_states) do 
    T.NewState = bad_state
    local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
    local error_code = get_error_code(hdrs)
    assert(error_code == 400)
  end
  --========= select a random good thing to do 
  local good_states = { "dormant", "started",  "archived" }
  math.randomseed(os.time())
  local idx = math.random(#good_states)
  local good_state = good_states[idx]
  print("moving from dormant to " .. good_state)
  T.NewState = good_state
  T.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  local Tchk = get_test_info(tid)
  --====================================
  print("Test t2 succeeded")
  return true
end
tests.t3 = function (
  just_pr
  )
  local description = "Testing set_state when started"
  if ( just_pr ) then print(description) return end

  -- START: Make some test 
  reset_db()
  local hdrs, outbody, status = mk_test("good_basic1.lua")
  assert(status == 200)
  local tid = get_test_id(hdrs)
  local T = get_test_info(tid)
  local tid = tonumber(T.id)
  -- Move state to dormant 
  T.NewState = "dormant"
  T.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  -- Move state to started 
  T.NewState = "started"
  T.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  --======== now do the bad things, in any order
  local bad_states = { "draft", "dormant" }
  for k, bad_state in pairs(bad_states) do 
    T.NewState = bad_state
    local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
    local error_code = get_error_code(hdrs)
    assert(error_code == 400)
  end
  --========= select a random good thing to do 
  local good_states = { "started",  "terminated", "archived" }
  math.randomseed(os.time())
  local idx = math.random(#good_states)
  local good_state = good_states[idx]
  if ( good_state == "terminated" ) then 
    local V = T.Variants
    local vidx = math.random(#V)
    T.Winner = V[vidx].name
    print("setting winner  to " .. T.Winner)
  end
  T.NewState = good_state
  print("moving from started to " .. good_state)
  T.Updater =  "joe" -- TODO Improve this hard coding
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  local Tchk = get_test_info(tid)
  --====================================
  print("Test t3 succeeded")
  return true
end
tests.t4 = function (
  just_pr
  )
  local old_state = "terminated"
  local description = "Testing set_state when " .. old_state
  if ( just_pr ) then print(description) return end

  -- START: Make some test 
  reset_db()
  local hdrs, outbody, status = mk_test("good_basic1.lua")
  assert(status == 200)
  local tid = get_test_id(hdrs)
  local T = get_test_info(tid)
  local tid = tonumber(T.id)

  T.Updater =  "joe" -- TODO Improve this hard coding
  -- Move state to dormant 
  T.NewState = "dormant"
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  -- Move state to started 
  T.NewState = "started"
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  -- Moving state to terminated
  local V = T.Variants
  local vidx = math.random(#V)
  T.Winner = V[vidx].name
  print("setting winner  to " .. T.Winner)
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  --======== now do the bad things, in any order
  local bad_states = { "draft", "dormant"  }
  for k, bad_state in pairs(bad_states) do 
    T.NewState = bad_state
    local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
    assert(status == 400)
  end
  --========= select a random good thing to do 
  local good_states = { "started",  "terminated", "archived" }
  math.randomseed(os.time())
  local idx = math.random(#good_states)
  local good_state = good_states[idx]
  T.NewState = good_state
  print("moving from " .. old_state .. " to " .. good_state)
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  local Tchk = get_test_info(tid)
  --====================================
  print("Test t4 succeeded")
  return true
end
tests.t5 = function (
  just_pr
  )
  local old_state = "archived"
  local description = "Testing set_state when " .. old_state
  if ( just_pr ) then print(description) return end

  -- START: Make some test 
  reset_db()
  local hdrs, outbody, status = mk_test("good_basic1.lua")
  assert(status == 200)
  local tid = get_test_id(hdrs)
  local T = get_test_info(tid)
  local tid = tonumber(T.id)

  T.Updater =  "joe" -- TODO Improve this hard coding
  -- Move state to dormant 
  T.NewState = "dormant"
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  -- Move state to started 
  T.NewState = "started"
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  -- Moving state to terminated
  local V = T.Variants
  local vidx = math.random(#V)
  T.Winner = V[vidx].name
  print("setting winner  to " .. T.Winner)
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  -- Move state to archived 
  T.NewState = "archived"
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  --======== now do the bad things, in any order
  local bad_states = { "draft", "dormant", "started", "terminated"  }
  for k, bad_state in pairs(bad_states) do 
    T.NewState = bad_state
    local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
    local error_code = get_error_code(hdrs)
    assert(error_code == 400)
  end
  --========= select a random good thing to do 
  local good_state = "archived"
  T.NewState = good_state
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  assert(status == 200)
  local Tchk = get_test_info(tid)
  --====================================
  print("Test t5 succeeded")
  return true
end
tests.t1() 
tests.t2() 
tests.t3() 
tests.t4() 
tests.t5() 
return tests
