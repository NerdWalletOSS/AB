local get_db_config = require 'test_webapp/get_db_config'
local db_connect    = require 'RTS/db_connect'

local function db_count(
  tbl,
  where
  )
  assert(type(tbl) == "string")

  local db, host, user, pass, port = get_db_config()
  local conn = db_connect(db, host, user, pass, port)
  local query_str = 'select count(*) from ' .. tbl
  if ( where ) then 
    assert(type(where) == "string")
    query_str = query_str .. " " .. where_str 
  end

  local T = conn:query(query_str)
  assert(type(T) == "table")
  assert(type(T[1]) == "table")
  local nR = T[1]["count(*)"]
  nR = assert(tonumber(nR))
  return nR
end
local dbg = false
if ( dbg ) then 
  local nR = db_count("variant")
  print(nR)
end
return db_count
