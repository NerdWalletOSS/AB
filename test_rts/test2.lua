-- local dbg = require 'lua/debugger'
local curl = require 'lua/curl'
local JSON = require 'lua/JSON'
local reload_db = require 'test_webapp/reload_db'
local plpath = require 'pl.path'

local ltc_url = "localhost:8000/ListTests?Source=C&TestType=XYTest"
local ltl_url = "localhost:8000/ListTests?Source=Lua&TestType=XYTest"
local dc_url = "localhost:8000/Diagnostics?Source=C"
local dl_url = "localhost:8000/Diagnostics?Source=Lua"

local php_url = "localhost:8080/AB/php/endpoints/endpoint_chk_tests.php"

local tests = {}
tests.t1 = function(
  num_iters,
  test_php
  )
  --=======================================
  if ( not num_iters) then num_iters = 10 end 
  if ( not test_php) then test_php = true end 

  reload_db(plpath.currentdir() .. "/abdb2_100.sql") -- TODO Improve path
  print("SQL Reload done")

  for i = 1, num_iters do 
    local a, b, c = curl.get("localhost:8000/Reload"); assert(c == 200)
    print("Reload done")
    a, b, c = curl.get(ltc_url);   assert(c == 200)
    local Lc = JSON:decode(b)

    a, b, c = curl.get(ltl_url);   assert(c == 200)
    local Ll = JSON:decode(b)
    assert(#Ll == #Lc)
    assert(#Ll == 25)
    print("List tests done")

    a, b, c = curl.get(dl_url);   assert(c == 200)
    a, b, c = curl.get(dc_url);   assert(c == 200)

    print("Diagnostics done")
    if ( test_php )  then
      a, b, c = curl.get(php_url);  
      print("PHP Check tests done")
      if ( c ~= 200 ) then
        print(i, b, c, php_url)
      end
      print(php_url)
      assert(c == 200)
    end
    print("iter " .. i)
  end
  print("Test t1 terminated")
end
tests.t1(1000, true) -- COMMENT once LJT starts working
return tests
