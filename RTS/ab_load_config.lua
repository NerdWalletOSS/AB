local assertx = require 'lua/assertx'
local cache = require 'lua/cache'
local consts = require 'lua/ab_consts'
local ffi = require 'lua/ab_ffi'
local JSON = require 'lua/JSON'
local sql = require 'lua/sql'

local load_cfg = {}

function load_cfg.db_connect(mysql)
  assert(mysql, "mysql not defined")
  local host = mysql.SERVER.VALUE
  local user = mysql.USER.VALUE
  local pass = mysql.PASSWORD.VALUE
  local port = tonumber(mysql.PORT.VALUE)
  local db = mysql.DATABASE.VALUE
  local conn = sql:connect(host, user, pass, db, port)
  assert(conn, "Unable to open connection to MySQL server")
  return conn
end

function load_cfg.load_db_data(config)
  assert(config, "config not defined")
  local conn = load_cfg.db_connect(config.AB.MYSQL)
  -- TODO execute the sql
  local res = conn:query('SELECT * FROM device ORDER BY id ASC;')
  assert(res, "cannot get devices")
  table.sort(res, function(a,b) return a.id < b.id end)
  local devs = {}
  for index, entry in ipairs(res) do
    devs[entry.id] = entry.name
  end
  cache.put("devices", devs)
  conn:close()
end

function load_cfg.load_config(config_file)
  local file = assert(io.open(config_file, 'r'), "Invalid filename given")
  local conf_str = file:read('*a')
  file:close()
  local config = JSON:decode(conf_str)
  assert(config.AB, "The loaded config must have an entry for AB")
  local db = config.AB.MYSQL
  assert(config.AB.MYSQL, "MYSQL entry cannot be nil")
  local mysql = config.AB.MYSQL
  assert(mysql ~= nil and type(mysql) == "table", "Config must be a table for mysql ")
  local host = mysql.SERVER.VALUE
  assert(host ~= nil and type(host) == "string" and #host > 0 , "Mysql entry must have a valid hostname")
  local user = mysql.USER.VALUE
  assert(user ~= nil and type(user) == "string"and #user > 0 , "Mysql entry must have a valid username")
  local pass = mysql.PASSWORD.VALUE
  assert(pass ~= nil and type(pass) == "string", "Mysql entry must have a valid password")
  local port = tonumber(mysql.PORT.VALUE)
  assert(port ~= nil and port >= 0 and port < 2^16 - 1, "Mysql entry must have a valid port")
  local db = mysql.DATABASE.VALUE
  assert(db ~= nil and type(db) == "string" and #db > 0, "Mysql entry must have a valid database")
  return config
end

return load_cfg
