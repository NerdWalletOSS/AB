local function get_db_config(
  )
  local db   = "abdb2" 
  local host = "localhost"
  local user = "root"
  local pass = ""
  local port = 3306
  return db, host, user, pass, port
end
return get_db_config
