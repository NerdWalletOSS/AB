local rand_selections = require 'test_webapp/rand_selections'
local mk_rand_test    = require 'test_webapp/mk_rand_test'
local S               = require 'test_webapp/state_change'
local reset_db        = require 'test_webapp/reset_db'
local db_count       = require 'test_webapp/db_count'

local T = {}
T.t1 = function(num_tests)
  math.randomseed(os.time())
  if ( not num_tests ) then num_tests = 100 end 
  reset_db()
  local cum_num_variants = 0
  for i = 1, num_tests do 
    local min_num_variants = 2
    local max_num_variants = 8
    local num_variants = math.random(min_num_variants, max_num_variants)
    local tid = mk_rand_test( { NumVariants = num_variants })
    local nV = db_count("variant")
    cum_num_variants = cum_num_variants + num_variants
    assert(nV == cum_num_variants)
  end
  print("Successfully added " .. num_tests .. " tests in test t1")
  print("TODO This test needs more work")
end 
T.t1(1000) -- TO DELETE once LJT starts working
return T
