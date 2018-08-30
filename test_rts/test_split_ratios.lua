local curl              = require 'lua/curl'
local JSON              = require 'lua/JSON'
local compare_T = require 'test_rts/compare_T'
local mk_input_for_dev_specific_test 
  = require 'test_webapp/mk_input_for_dev_specific_test'

local gv_url = "localhost:8000/GetVariant?TestType=XYTest&TestName="
local ti_url = "localhost:8000/TestInfo?TestType=XYTest&TestName="
local lt_url = "localhost:8000/ListTests?TestType=XYTest"
local dc_url = "localhost:8000/Diagnostics"

local tests = {}
tests.t1 = function(
  is_dev_specific,
  num_iters
  )
  if not num_iters then num_iters = 1000 end 
  if ( is_dev_specific == false ) then 
    a, b, c = curl.get("localhost:8000/Restart"); assert(c == 200)
    local at_url = "localhost:8000/AddTest"
    local H = nil
    local B = require 'test_rts/g1'
    assert(type(B) == "table")
    local test_name = assert(B.name)
    a, b, c = curl.get(at_url, H, JSON:encode(B)); assert(c == 200)
    a, b, c = curl.get(lt_url); assert(c == 200)
    tests = JSON:decode(b)
    assert(#tests == 1)
    assert(tests[1] == test_name)
    --=======================================
  else

  gv_url = gv_url .. test_name
  ti_url = ti_url .. test_name
  local actual_counts = {}
  local expected_counts = {}
  for i, v in pairs(B.Variants) do
    expected_counts[v.name] = v.percentage
    actual_counts[v.name] = 0
    --[[
    print(v.name .. " .. " .. expected_counts[v.name] .. " => " 
                           .. actual_counts[v.name])
    --]]
  end
  for i = 1, num_iters do 
    a, b, c = curl.get(gv_url, H, JSON:encode(B)); assert(c == 200)
    local x = assert(JSON:decode(b))
    assert(x.Variant)
    assert(x.VariantID)
    assert(x.URL)
    assert(x.GUID)
    a, b, c = curl.get(dc_url); assert(c == 200)

    a, b, c = curl.get(ti_url, H, JSON:encode(B)); assert(c == 200)
    local z = assert(JSON:encode(b))
  end
  a, b, c = curl.get(ti_url, H, JSON:encode(B)); assert(c == 200)
  local z = assert(JSON:decode(b))
  for k, v in pairs(actual_counts) do print(k, v) end 
  for k, v in pairs(z.Variants) do
    actual_counts[v.name] = v.count
    print("expected = " .. expected_counts[v.name] 
    .. " actual = " .. actual_counts[v.name])
    local act = actual_counts[v.name] / num_iters
    local exp = expected_counts[v.name] / 100.0
    assert( ( ( act >= exp - 0.02 ) and  ( act <= exp + 0.02 ) ),
      "Mismatch expected = " .. exp .. " actual = " .. act)
  end

  print("Test t1 succeeded")
end
tests.t1(false, 1000)
tests.t1(true, 1000)
return tests
