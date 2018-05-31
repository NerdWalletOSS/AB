curl = require 'lua/curl'
JSON = require 'lua/JSON'

local lt_url = "localhost:8000/ListTests?Source=C&TestType=XYTest"
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
  -- TODO RS a, b, c = curl.get("localhost:8000/Restart"); assert(c == 200)
  states = { "started", "terminated", "archived" }
  for _, state in ipairs(states) do
    for i = 1, num_tests do 
      local j = 1
      for k, v in pairs(B.Variants) do
        v.id = i*1000 + j
        j = j + 1
      end
      B.name = "Test_" .. i
      B.id = i
      B.external_id = i
      B.seed = i
      B.state = state
      jB = JSON:encode(B)
      a, b, c = curl.get(U, H, jB); assert(c == 200)
      -- TODO IS a, b, c = curl.get(dc_url);   -- assert(c == 200)
      -- TODO IS a, b, c = curl.get(dl_url);   -- assert(c == 200)
      a, b, c = curl.get(lt_url);   assert(c == 200)
      local L = JSON:decode(b)
      if ( state == "started" ) then
        assert(#L == i) 
      elseif ( state == "terminated" ) then
        assert(#L == num_tests) 
      elseif ( state == "archived" ) then
        print(#L, state)
        --TODO IS assert(#L == (num_tests - i))
      else
        assert(nil)
      end
    end

  end
  print("Test t1 succeeded")
end
tests.t1(100) -- TO COMMENT OUT
return tests
