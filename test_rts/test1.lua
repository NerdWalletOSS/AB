local curl              = require 'lua/curl'
local JSON              = require 'lua/JSON'
local compare_test_info = require 'test_rts/compare_test_info'

local tic_url = "localhost:8000/TestInfo?Source=C&TestType=XYTest"
local til_url = "localhost:8000/TestInfo?Source=Lua&TestType=XYTest"

local ltc_url = "localhost:8000/ListTests?Source=C&TestType=XYTest"
local ltl_url = "localhost:8000/ListTests?Source=C&TestType=XYTest"

local dc_url = "localhost:8000/Diagnostics?Source=C"
local dl_url = "localhost:8000/Diagnostics?Source=Lua"

local tests = {}
tests.t1 = function(
  num_tests
  )
  local U = "localhost:8000/AddTest"
  local H = nil
  local B = require 'test_rts/g1'
  assert(type(B) == "table")
  if not num_tests then num_tests = 1 end 
  --=======================================
  a, b, c = curl.get("localhost:8000/Restart"); assert(c == 200)
  states = { "started", "terminated", "archived" }
  for _, state in ipairs(states) do
    for i = 1, num_tests do 
      print(i, state)
      local j = 1
      for k, v in pairs(B.Variants) do
        v.id = i*1000 + j
        j = j + 1
      end
      B.name = "Test_" .. i
      B.id = i
      B.external_id = i
      B.seed = i
      B.State = state
      jB = JSON:encode(B)
      a, b, c = curl.get(U, H, jB); assert(c == 200)
      a, b, c = curl.get(dc_url);   assert(c == 200)
      -- TODO PUT BACK a, b, c = curl.get(dl_url);   assert(c == 200)
      local Tc = nil -- Tc = test info using C
      local url = tic_url .. "&TestName=" .. B.name
      a, b, c = curl.get(url)
      if ( state == "archived" ) then 
        print(url)
        assert(c ~= 200)
      else
        assert(c == 200)
        local Tc = JSON:decode(b)
        assert(Tc.State == state, 
        "expected " .. state .. " got " .. Tc.State)
      end
     
      local Tl = nil -- test info using Lua
      --[[ TODO PUT THIS BLOCK BACK IN
      local url = til_url .. "&TestName=" .. B.name
      a, b, c = curl.get(til_url .. "&TestName=" .. B.name);   
      if ( state == "archived" ) then 
        assert(c ~= 200)
      else
        assert(c == 200)
        local Tl = JSON:decode(b)
        assert(Tc.State == state)
      end
      --]]
    
      if ( Tl and Tc ) then 
        assert(compare_test_info(Tl, Tc))
      end

      -- Lc = list tests using C 
      a, b, c = curl.get(ltc_url);   assert(c == 200)
      local Lc = JSON:decode(b)

      -- Ll = list tests using Lua
      --[[ TODO PUT BACK
      a, b, c = curl.get(ltl_url);   assert(c == 200)
      local Ll = JSON:decode(b)
      -- Lc should be same as Ll
      assert(#Ll == #Lc)
      --]]

      -- check number of tests
      if ( state == "started" ) then
        assert(#Lc == i) 
      elseif ( state == "terminated" ) then
        assert(#Lc == num_tests) 
      elseif ( state == "archived" ) then
        print(#Lc, state)
        --TODO IS assert(#L == (num_tests - i))
      else
        assert(nil)
      end
    end

  end
  print("Test t1 succeeded")
end
tests.t1(333) -- TO COMMENT OUT
return tests
