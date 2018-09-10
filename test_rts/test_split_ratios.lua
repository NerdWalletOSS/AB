local curl              = require 'lua/curl'
local JSON              = require 'lua/JSON'
local compare_T = require 'test_rts/compare_T'
local mk_dev_specific_test = require 'test_rts/mk_dev_specific_test'

local gv_url = "localhost:8000/GetVariant?TestType=XYTest&TestName="
local ti_url = "localhost:8000/TestInfo?TestType=XYTest&TestName="
local lt_url = "localhost:8000/ListTests?TestType=XYTest"
local dc_url = "localhost:8000/Diagnostics"

local tests = {}
local function x(
  is_dev_specific,
  num_iters
  )
  if not num_iters then num_iters = 1000 end 
  local tid, test_name
  local a, b, c 
  local Variants
  local T
  if ( is_dev_specific == false ) then 
    a, b, c = curl.get("localhost:8000/Restart"); assert(c == 200)
    local at_url = "localhost:8000/AddTest"
    local H = nil
    local B = require 'test_rts/g1'
    assert(type(B) == "table")
    test_name = assert(B.name)
    a, b, c = curl.get(at_url, nil, JSON:encode(B)); assert(c == 200)
    a, b, c = curl.get(lt_url); assert(c == 200)
    tests = JSON:decode(b)
    assert(#tests == 1)
    assert(tests[1] == test_name)
    --=======================================
  else
    tid, test_name = mk_dev_specific_test()
  end
  ti_url = ti_url .. test_name
  print(ti_url)
--   a, b, c = curl.get(ti_url, nil, JSON:encode(B)); assert(c == 200)
   a, b, c = curl.get(ti_url); assert(c == 200)
  T = assert(JSON:decode(b))
  local original_gv_url = gv_url .. test_name

  local Devices = require '../test_webapp/devices'
  for i, device in pairs(Devices) do 
    local gv_url = original_gv_url
    if ( is_dev_specific == true ) then 
      gv_url = original_gv_url .. "&Device=" .. device
    end 
    if ( ( is_dev_specific == false ) and ( i > 1 ) ) then 
      goto done
    end
    print(gv_url)
    local actual_counts = {}
    local expected_counts = {}
    for i, v in pairs(T.Variants) do
      expected_counts[v.name] = v.percentage
      actual_counts[v.name] = 0
      --[[
      print(v.name .. " .. " .. expected_counts[v.name] .. " => " 
                             .. actual_counts[v.name])
      --]]
    end
    -- START: Hit the server a number of times
    for i = 1, num_iters do 
      a, b, c = curl.get(gv_url); assert(c == 200)
      local x = assert(JSON:decode(b))
      assert(x.Variant)
      assert(x.VariantID)
      assert(x.URL)
      assert(x.GUID)
      a, b, c = curl.get(dc_url); assert(c == 200)
  
      a, b, c = curl.get(ti_url); assert(c == 200)
      local z = assert(JSON:encode(b))
    end
    -- STOP: Hit the server a number of times
    a, b, c = curl.get(ti_url); assert(c == 200)
    local z = assert(JSON:decode(b))
    local Variants = assert(z.Variants)
    if ( is_dev_specific == true ) then 
      z = assert(z.DeviceCrossVariant)
      z = assert(z[device])
      for vname, count in pairs(z) do 
        actual_counts[vname] = count
      end 
    else
      for k, v in pairs(z.Variants) do
        actual_counts[v.name] = v.count
      end
    end
    for k, v in pairs(Variants) do
      print("expected = " .. expected_counts[v.name] 
        .. " actual = " .. actual_counts[v.name])
      local act = actual_counts[v.name] / num_iters
      local exp = expected_counts[v.name] / 100.0
      if ( expected_counts[v.name] == 0 )  then 
        assert(actual_counts[v.name] == 0 )
      end
      -- TODO P4: Can improve this comparison
      assert( ( ( act >= exp - 0.05 ) and  ( act <= exp + 0.05 ) ),
        "Mismatch expected = " .. exp .. " actual = " .. act)
    end
    ::done::
  end

  print("Test t1 succeeded")
end
x(true, 1000)
x(false, 1000)
tests.t1 = x
return tests
