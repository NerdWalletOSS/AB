local JSON          = require 'lua/JSON'
local get_test_info = require 'test_webapp/get_test_info'
local mk_rand_test  = require 'test_webapp/mk_rand_test'
local S             = require 'test_webapp/state_change'
local R             = require 'test_webapp/rand_selections'
local reset_db      = require 'test_webapp/reset_db'
local set_dev_specific      = require 'test_webapp/set_dev_specific'

local function did_changes_stick(out, tid, D)
  local T = get_test_info(tid)
  local DxV = assert(T.DeviceCrossVariant)
  local num_checks = 0
  for k1, v1 in pairs(DxV) do 
    for k2, v2 in pairs(v1) do 
      -- print(k1, v2.variant_id, v2.device_id ,v2.percentage)
      for k3, v3 in pairs(out.DeviceCrossVariant) do 
        for k4, v4 in pairs(v3) do
          --[[
         print(v2.variant_id, v2.device_id ,v2.percentage, 
               v4.variant_id, v4.device_id ,v4.percentage)
               --]]
          if ( ( tonumber(v4.device_id) == tonumber(v2.device_id) ) and 
               ( tonumber(v4.variant_id) == tonumber(v2.variant_id) ) ) then
             assert(tonumber(v4.percentage) == tonumber(v2.percentage) )
             num_checks = num_checks + 1
          end
        end
      end
    end
  end
  assert(num_checks > 0)
  assert(num_checks == #D * T.NumVariants, "num_checks = " .. num_checks)
end

local function mk_input_for_test(tid, D, V, nV)
  local out = {}
  local X = {}
  for device_id, d in pairs(D) do 
    local t = {}
    local P = assert(R.rand_perc("XYTest", nV))
    for k, v in pairs(V) do
      t[k] = {}
      assert(P[k], " k = " .. k .. " #P = " .. #P)
      t[k].percentage = P[k]
      assert(device_id)
      t[k].device_id = device_id
      assert(v.id)
      t[k].variant_id = assert(tonumber(v.id))
    end
    X[d] = t
  end
  out.DeviceCrossVariant = X
  out.id = tid
  return out
end

local function hdrs_contains(hdrs, msg)
  assert(type(msg) == "string")
  local contains = false
  for k, v in pairs(hdrs) do 
    if ( string.find(v, msg) ) then 
      contains = true
    end
  end
  return contains
end

local function xtest(State)
  local tid = mk_rand_test({ TestType = "XYTest"})
  local T = get_test_info(tid)
  if ( State == "draft" ) then 
    -- do nothing
  elseif ( State == "dormant" ) then 
    S.publish(tid)
  elseif ( State == "started" ) then 
    S.publish(tid)
    S.start(tid)
  elseif ( State == "terminated" ) then 
    S.publish(tid)
    S.start(tid)
    S.terminate(tid)
  elseif ( State == "archived" ) then 
    S.publish(tid)
    S.start(tid)
    S.terminate(tid)
    S.archive(tid)
  end
  T = get_test_info(tid)
  assert(T.State == State, T.State .. "  " .. State)
  print("Lua: State == " .. State)
  local D = require 'devices'
  local V = T.Variants
  local nV = T.NumVariants
  local out = mk_input_for_test(tid, D, V, nV)
  assert(tonumber(T.is_dev_specific) == 0)
  -- if is_dev_specific == false, no change should be made
  out.is_dev_specific = false
  local hdrs, body, status = set_dev_specific(out)
  if ( ( State == "terminated" ) or ( State == "archived" ) ) then
    assert(status == 400)
    assert(hdrs_contains(hdrs, "NO CHANGE TO DEVICE SPECIFIC"));
  elseif ( State == "started" ) then 
    assert(hdrs_contains(hdrs, "NO CHANGE TO IS DEVICE SPECIFIC"));
    assert(status == 200)
  elseif ( ( State == "dormant" ) or ( State == "draft" ) ) then
    assert(status == 200)
    assert(hdrs_contains(hdrs, "DEVICE SPECIFIC SET OFF"));
    out.is_dev_specific = true
    local hdrs, body, status = set_dev_specific(out)
    assert(status == 200)
    assert(hdrs_contains(hdrs, "SET Device Specific"))
    did_changes_stick(out, tid, D)
  end
end

--=========================================
local T = {}
T.t1 = function()
  local States = require 'states'
  xtest("started")
  for State, state_id in pairs(States) do 
    xtest(State)
  end
  print("Test t1 succeeded")
end 
T.t1() -- TO DELETE once LJT starts working
return T
