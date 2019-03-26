local curl              = require 'lua/curl'
local JSON              = require 'lua/JSON'
local compare_T = require 'test_rts/compare_T'
<<<<<<< HEAD

local tic_url = "localhost:8000/TestInfo?Source=C&TestType=XYTest"
local til_url = "localhost:8000/TestInfo?Source=Lua&TestType=XYTest"

local ltc_url = "localhost:8000/ListTests?Source=C&TestType=XYTest"
local ltl_url = "localhost:8000/ListTests?Source=C&TestType=XYTest"

local dc_url = "localhost:8000/Diagnostics?Source=C"
local dl_url = "localhost:8000/Diagnostics?Source=Lua"
=======
>>>>>>> dev

local tests = {}
tests.t1 = function(
  num_tests
  )
  -- START set up some useful URLs
  local tic_url = "localhost:8000/TestInfo?Source=C&TestType=XYTest"
  local til_url = "localhost:8000/TestInfo?Source=Lua&TestType=XYTest"
  
  local ltc_url = "localhost:8000/ListTests?Source=C&TestType=XYTest"
  local ltl_url = "localhost:8000/ListTests?Source=C&TestType=XYTest"
  
  local dc_url = "localhost:8000/Diagnostics?Source=C"
  local dl_url = "localhost:8000/Diagnostics?Source=Lua"
  
  local at_url = "localhost:8000/AddTest"
  -- STOP  set up some useful URLs
  local H = nil
  local B = require 'test_rts/g1'
  assert(type(B) == "table")
  assert(B.NumVariants)
  if not num_tests then num_tests = 1 end 
  --=======================================
  a, b, c = curl.get("localhost:8000/Restart"); assert(c == 200)
  states = { "started", "terminated", "archived" }
  for _, state in ipairs(states) do
    for i = 1, num_tests do 
      -- print(i, state)
      local j = 1
      for k, v in pairs(B.Variants) do
        v.id = i*1000 + j
        j = j + 1
        if ( ( state == "terminated" ) and ( k == 1 ) ) then
          v.is_final = 1 
        end
      end
      B.name = "Test_" .. i
      B.id = tostring(i)
      B.external_id = tostring(i)
      B.seed = tostring(i)
      B.State = state
      jB = JSON:encode(B)
      a, b, c = curl.get(at_url, H, jB); assert(c == 200)
      a, b, c = curl.get(dc_url);   assert(c == 200)
      a, b, c = curl.get(dl_url);   assert(c == 200)
      local Tc = nil -- Tc = test info using C
      local url = tic_url .. "&TestName=" .. B.name
      a, b, c = curl.get(url)
      if ( state == "archived" ) then 
        assert(c ~= 200)
      else
        assert(c == 200)
        Tc = JSON:decode(b)
        assert(Tc.State == state, 
        "expected " .. state .. " got " .. Tc.State)
      end
     
      local Tl = nil -- test info using Lua
      
      local url = til_url .. "&TestName=" .. B.name
      a, b, c = curl.get(til_url .. "&TestName=" .. B.name);   
      if ( state == "archived" ) then 
        assert(c ~= 200)
      else
        assert(c == 200)
        Tl = JSON:decode(b)
        assert(Tc.State == state)
      end
     
    
      if ( Tl and Tc ) then 
        assert(compare_T(Tl, Tc))
      end

      -- Lc = list tests using C 
      a, b, c = curl.get(ltc_url);   assert(c == 200)
      local Lc = JSON:decode(b)

      -- Ll = list tests using Lua
      

      a, b, c = curl.get(ltl_url);   assert(c == 200)
      local Ll = JSON:decode(b)
      assert(#Ll == #Lc)
     


      -- check number of tests
      if ( state == "started" ) then
        assert(#Lc == i) 
      elseif ( state == "terminated" ) then
        assert(#Lc == num_tests) 
      elseif ( state == "archived" ) then
        -- print(#Lc, state)
        assert(#Lc == (num_tests - i))

      else
        assert(nil)
      end
    end

  end
  print("Test t1 succeeded")
end
tests.t1(1000) -- TO COMMENT OUT
return tests
