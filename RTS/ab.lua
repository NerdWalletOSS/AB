-- ab.lua is the only place where we have globals rest all use locals.
-- All function call from C are registered here.
-- Also the globals are functions.
local x_hard_code_config = require 'lua/hard_code_config'
local x_load_config = require 'RTS/ab_load_config'
local x_update_config = require 'RTS/ab_update_config'
local x_diagnostics = require 'RTS/diagnostics'
local x_list_tests = require 'RTS/list_tests'
local x_add_test = require 'RTS/add_test'
local reload_tests = require 'RTS/reload'
local reload_tests = require 'RTS/reload'
local x_check_db_conn = require 'RTS/check_db_conn'
local x_get_test_info = require 'RTS/get_test_info'
local x_get_config = require 'RTS/get_config'



function load_config(...)
  x_load_config.load_config(...)
end

function update_config(...)
  x_update_config(...)
end
--=================================================
function preproc(...)
  x_add_test.preproc(...)
end

function add(...)
  x_add_test.add(...)
end


function list_tests()
  return x_list_tests()
end

function reload(...)
  return reload_tests.reload(...)
end

function check_db_conn()
  x_check_db_conn()
end

function get_config()
  return x_get_config()
end

function get_test_info(args)
    return x_get_test_info(args)
end

function diagnostics(...)
  x_diagnostics()
end
