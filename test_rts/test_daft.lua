local curl      = require 'lua/curl'
local JSON      = require 'lua/JSON'
local compare_T = require 'test_rts/compare_T'
local test_daft = require 'test_webapp/test_daft'
local rand_set  = require 'test_rts/rand_set'
local rand_test = require 'test_rts/rand_test'

local at_url = "localhost:8000/AddTest"

local tic_url = "localhost:8000/TestInfo?Source=C&TestType=XYTest"
local ltc_url = "localhost:8000/ListTests?Source=C&TestType=XYTest"
local dc_url  = "localhost:8000/Diagnostics?Source=C"

local niters = 100
local tests = {}
tests.t1 = function(num_iters, num_hits)
  if ( not num_iters ) then num_iters = 100 end 
  if ( not num_hits ) then num_hits   = 100 end 
  local a, b, c = curl.get("localhost:8000/Restart"); assert(c == 200)
  for i = 1, num_iters do 
    local choice = rand_set("rts_endpoints")
    if ( choice == "add_test" ) then 
      local T = rand_test()
      a, b, c = curl.get(at_url, nil, JSON:encode(T))
    elseif ( choice == "diagnostics" ) then 
      local a, b, c = curl.get(dc_url); assert(c == 200)

    else
      local a, b, c = curl.get(ltc_url); assert(c == 200)
    end

  end
  print("Test t1 succeeded")
end
tests.t1(10000)
return tests
