-- local dbg = require 'lua/debugger'
local curl = require 'lua/curl'
local JSON = require 'lua/JSON'
local reload_db = require 'test_webapp/reload_db'
local plpath = require 'pl.path'

local ltc_url = "localhost:8000/ListTests?Source=C&TestType=XYTest"
local ltl_url = "localhost:8000/ListTests?Source=Lua&TestType=XYTest"
local dc_url = "localhost:8000/Diagnostics?Source=C"
local dl_url = "localhost:8000/Diagnostics?Source=Lua"

local tests = {}
tests.t1 = function(
  num_iters
  )
  --=======================================
  if ( not num_iters) then num_iters = 10 end 
  x = plpath.currentdir()
  print(x)
  reload_db(plpath.currentdir() .. "/abdb2_100.sql") -- TODO Improve path

  for i = 1, num_iters do 
    local exp_tests = {}
    for i = 1, 100 do 
      exp_tests["Test_" .. i] = 0
    end
    local a, b, c = curl.get("localhost:8000/Reload"); assert(c == 200)
    a, b, c = curl.get(ltc_url);   assert(c == 200)
    local Lc = JSON:decode(b)

    a, b, c = curl.get(ltl_url);   assert(c == 200)
    local Ll = JSON:decode(b)
    assert(#Ll == #Lc)
    -- assert(#Ll == 100)

    -- TODO P2 This is very basic checking. Can do better
    for k, t in pairs(Ll) do
      assert(type(t) == "table")
      assert(exp_tests[t.name])
      assert( exp_tests[t.name] == 0 ) 
      exp_tests[t.name] = 1 
      local tname = t.name
    end

    a, b, c = curl.get(dl_url);   assert(c == 200)
    a, b, c = curl.get(dc_url);   assert(c == 200)
  end
end
tests.t1(10)
return tests
