local get_test_info = require 'test_webapp/get_test_info'
local test_daft     = require 'test_webapp/test_daft'
local JSON          = require 'lua/JSON'
local n = 10
test_daft.t1(n) -- create n tests
local tid = 1 -- we know that IDs are 1, 2, 3, ... when db has been reset
-- get test info about test ID = tid = 1
local T, hdrs = get_test_info(tid)
-- write this to a file 
local plfile = require 'pl.file'
local out_file_name = "_x.json"
plfile.write(out_file_name, JSON:encode(T))
print "all done"
