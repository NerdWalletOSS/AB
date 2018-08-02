require 'lua/str'
require 'RTS/ab'
local get_test      = require 'RTS/get_test'
local get_db_config = require 'test_webapp/get_db_config'
local load_aux      = require 'RTS/load_aux'
local mk_dev_spec_perc      = require 'test_webapp/mk_dev_spec_perc'
--==========================
local tests = {}
tests.t1 = function (
  just_pr
  )
  local description = "XXX TODO TODO P3 "
  if ( just_pr ) then print(description) return end

  local db, host, user, pass, port = get_db_config()
  load_aux(db, host, user, pass, port)

  local tid = 1 -- THIS IS HARD CODED
  local nV = 6  -- THIS IS HARD CODED
  local P = {}
  for i = 1, nV do P[i] = 10 + i end
  local tid, T = get_test(db, host, user, pass, port, tid, true)
  mk_dev_spec_perc(T.Variants, P)
end
tests.t1()
return tests
