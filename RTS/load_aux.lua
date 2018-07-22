local assertx    = require 'lua/assertx'
local ffi        = require 'lua/ab_ffi'
local consts     = require 'lua/ab_consts'
local db_connect = require 'RTS/db_connect'
local JSON       = require 'lua/JSON'

local function mk_lkp(X)
  tmp = {}
  for _,v in pairs(X) do
    tmp[v.id] = v.name
  end
  return tmp
end

local function load_aux(
  db, 
  host,
  user,
  pass,
  port
  )
  assert(type(aux) == "table", "Global aux table missing")
  assert(type(db) == "string")
  assert(type(host) == "string")
  assert(type(user) == "string")
  assert(type(pass) == "string")
  assert(type(port) == "number")

  local conn = db_connect(db, host, user, pass, port)
  local tmp = {}
  if ( not aux.states ) then 
    local states = conn:query('select id, name from state;')
    aux.states = mk_lkp(states)
  end

  if ( not aux.test_type ) then 
    local test_type = conn:query('select id, name from test_type;')
    aux.test_type = mk_lkp(test_type)
  end

  if ( not aux.bin_type ) then 
    local bin_type = conn:query('select id, name from bin_type;')
    aux.bin_type = mk_lkp(bin_type)
  end

  if ( not aux.device ) then 
    local device = conn:query('select id, name from device;')
    aux.device = mk_lkp(device)
  end
  conn:close()
end
return load_aux
