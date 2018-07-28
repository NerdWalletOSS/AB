local assertx = require 'lua/assertx'
local sql     = require 'lua/sql'

local function db_connect(
  db, 
  host, 
  user, 
  pass, 
  port
  )
  local conn = assert(sql:connect(host, user, pass, db, port),
    "Unable to open connection to MySQL server")
  return conn
end
return db_connect
