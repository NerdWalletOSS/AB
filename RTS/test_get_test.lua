local assertx   = require 'lua/assertx'
local get_test  = require 'RTS/get_test'
local reload_db = require 'test_webapp/reload_db'
local plpath    = require 'pl.path'

local db    = "abdb2"
local host  = "localhost"
local user  = "root"
local pass  = ""
local port = 3306
local dbg = true
local tid = 0
local test_as_str

local tests = {}
tests.t1 = function(
  sqldump, 
  correct_num_tests
  )
  if ( not sqldump ) then 
    sqldump = "abdb1.sql"
    correct_num_tests = 0
  end
  assert(type(sqldump) == "string")
  assert(type(correct_num_tests) == "number")
  assert(reload_db(plpath.currentdir() .. "/" .. sqldump))
  local num_tests = 0
  repeat 
    tid, test_as_str = get_test( db, host, user, pass, port, tid, dbg)
    if ( not tid ) then break end 
    num_tests = num_tests + 1 
  until tid ==  0
  assert(num_tests == correct_num_tests, "Got " .. num_tests)
  print("Test t1 succeeded")
end
tests.t1("abdb1.sql", 9) -- TO DELETE LATER
return tests
