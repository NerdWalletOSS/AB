curl = require 'lua/curl'
JSON = require 'lua/JSON'

local tests = {}
tests.t1 = function(
  num_tests
  )
  local U = "localhost:8000/AddTest"
  local H = nil
  local B = require 'test_rts/g1'
  assert(type(B) == "table")
  if not num_tests then num_tests = 1 end 
  for i = 1, num_tests do 
    B.name = "Test_" .. i
    B.id = i
    B.external_id = i
    B.seed = i
    jB = JSON:encode(B)
    print(jB)
    a, b, c = curl.get(U, H, jB)
    assert(c == 200)
    print("added test ", i)
  end
  print("Test t1 succeeded")
end
tests.t1(10) -- TO COMMENT OUT
return tests
