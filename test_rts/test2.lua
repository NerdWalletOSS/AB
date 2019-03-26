-- local dbg = require 'lua/debugger'
local curl = require 'lua/curl'
local JSON = require 'lua/JSON'
local reload_db = require 'test_webapp/reload_db'
local plpath = require 'pl.path'

local ltc1_url = "localhost:8000/ListTests?Source=C&TestType=ABTest"
local ltc2_url = "localhost:8000/ListTests?Source=C&TestType=XYTest"
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
  if ( type(test_php) ~= "boolean" ) then 
    test_php = false
  end

<<<<<<< HEAD
    if ( test_php )  then
      a, b, c = curl.get(php_url);  
      if ( c ~= 200 ) then
        print(i, b, c, php_url)
      end
      assert(c == 200)
=======
  local samples = { "abdb2_6", "abdb2_100", "abdb2_1000" }
  -- samples = { "abdb2_6" }
  for _, filename in ipairs(samples) do 
    -- TODO Improve file path so that it can be executed from anywhere
    local sqlfile = plpath.currentdir() .. "/" .. filename .. ".sql"
    assert(plpath.isfile(sqlfile), "File not found " .. sqlfile)
    reload_db(sqlfile)
  
    for i = 1, num_iters do 
      local a, b, c = curl.get("localhost:8000/Reload"); assert(c == 200)
      a, b, c = curl.get(ltc1_url);   assert(c == 200)
      local Lc1 = JSON:decode(b)
      a, b, c = curl.get(ltc2_url);   assert(c == 200)
      local Lc2 = JSON:decode(b)
  
      a, b, c = curl.get(dc_url);   assert(c == 200)
  
      if ( test_php )  then
        a, b, c = curl.get(php_url);  
        if ( c ~= 200 ) then
          print(i, b, c, php_url)
        end
        assert(c == 200)
      end
      -- print("iter " .. i)
>>>>>>> dev
    end
    -- print("filename =  " .. filename)
  end
  print("Test t1 succeeded")
end
<<<<<<< HEAD
tests.t1(1000, true) -- COMMENT once LJT starts working
=======
tests.t1(10, false) -- COMMENT once LJT starts working
>>>>>>> dev
return tests
