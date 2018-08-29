local JSON          = require 'lua/JSON'
local get_test_info = require 'test_webapp/get_test_info'
local mk_rand_test  = require 'test_webapp/mk_rand_test'
local S             = require 'test_webapp/state_change'
local R             = require 'test_webapp/rand_selections'
local reset_db      = require 'test_webapp/reset_db'
local set_dev_specific      = require 'test_webapp/set_dev_specific'

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

local function chk_is_dev_specific(hdrs)
  local msg 
  local is_dev_specific 
  for k, v in pairs(hdrs) do 
    if ( string.find(v, "Device Specific") ) then
      if ( string.find(v, "NOT") ) then  
        is_dev_specific  = false
      end
      if ( string.find(v, "SET") ) then  
        is_dev_specific  = true
      end
    end
  end
  assert(is_dev_specific ~= nil)
  return is_dev_specific
end

local function xtest(tid, D, V, nV)
  local out = mk_input_for_test(tid, D, V, nV)
  -- if is_dev_specific == false, no change should be made
  out.is_dev_specific = false
  local hdrs, body, status = set_dev_specific(out)
  assert(status == 200)
  assert(chk_is_dev_specific(hdrs) == false)
  -- if is_dev_specific == false, change should be made
  out.is_dev_specific = true
  local hdrs, body, status = set_dev_specific(out)
  assert(status == 200)
  assert(chk_is_dev_specific(hdrs) == true)
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
  assert(num_checks == #D * nV, "num_checks = " .. num_checks)
end

--=========================================
local T = {}
T.t1 = function()
  local tid = mk_rand_test({ TestType = "XYTest"})
  local T = get_test_info(tid)
  local D = require 'devices'
  local V = T.Variants
  local nV = T.NumVariants
  xtest(tid, D, V, nV)
  -- repeat when test is in dormant mode
  S.publish(tid)
  xtest(tid, D, V, nV)
  -- once test started, no changes possible
  S.start(tid)
  local status = pcall(xtest, tid, D, V, nV)
  assert(not status)
  -- once test terminated, no changes possible
  S.terminate(tid)
  local status = pcall(xtest, tid, D, V, nV)
  assert(not status)
  
  print("Test t1 succeeded")
end 
T.t1() -- TO DELETE once LJT starts working
return T
