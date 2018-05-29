curl = require 'lua/curl'
JSON = require 'lua/JSON'

local tests = {}
tests.t1 = function()
  U = "localhost:8000/AddTest"
  H = nil
  B = require 'test_webapp/good_basic1'
  a, b, c = curl.get(U, H, JSON:encode(B))
  print("Test t1 succeeded")
end
tests.t1() -- TO COMMENT OUT
return tests
