local curl      = require 'lua/curl'
local JSON      = require 'lua/JSON'
local reload_db = require 'test_webapp/reload_db'
local plpath    = require 'pl.path'

local tests = {}
tests.t1 = function(
  num_trials
  )
  if ( not num_trials ) then num_trials = 10 end 
  a, b, c = curl.get("localhost:8000/Restart"); assert(c == 200)
<<<<<<< HEAD
  assert(reload_db(plpath.currentdir() .. "/abdb_6.sql"))
=======
  assert(reload_db(plpath.currentdir() .. "/abdb2_6.sql"))
>>>>>>> dev
  -- TODO Improve path
  for i = 1, num_trials do 
    a, b, c = curl.get("localhost:8000/Reload"); 
    assert(c == 200)
    a, b, c = curl.get("localhost:8000/Diagnostics?Source=C"); 
    assert(c == 200)
<<<<<<< HEAD
=======
    -- print("iter " .. i)
>>>>>>> dev
  end
  print("Test t1 succeeded")
end
tests.t1(1000) -- TO COMMENT OUT
return tests
