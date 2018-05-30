curl = require 'lua/curl'
JSON = require 'lua/JSON'

local tests = {}
tests.t1 = function()
  U = "localhost:8000/AddTest"
  H = nil
  B = JSON:encode(require 'test_rts/g1')
  --[[
  --Did some debugging and realized that the post is being json 
  --encoded twice.  One by curl and once by the program calling it. 
  --Hence thew ab.lua calls decode and gets a string and not a table. 
  --I will insert a check for this in ab, but we need to fix this too.
  --]]
  -- a, b, c = curl.get(U, H, JSON:encode(B))
  a, b, c = curl.get(U, H, B)
  assert(c == 200)
  print("Test t1 succeeded")
end
tests.t1() -- TO COMMENT OUT
return tests
