local assertx  = require 'lua/assertx'
local get_test = require 'RTS/get_test'

local db    = "abdb2"
local host  = "localhost"
local user  = "root"
local pass  = ""
local port = 3306
local test_as_str = ""
local dbg = true
local tinfo = get_test( db, host, user, pass, port, test_as_str, dbg)
print(tinfo)

