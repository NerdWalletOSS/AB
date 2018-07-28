local assertx = require 'lua/assertx'
local cache = require 'lua/cache'
local consts = require 'lua/ab_consts'
local ffi = require 'lua/ab_ffi'
local JSON = require 'lua/JSON'
local sql = require 'lua/sql'

local load_cfg = {}

function load_cfg.db_connect(db, host, user, pass, port)
  local conn = assert(sql:connect(host, user, pass, db, port),
    "Unable to open connection to MySQL server")
  return conn
end

function load_cfg.load_db_data(config)
  assert(config, "config not defined")
  local conn = load_cfg.db_connect(config.AB.MYSQL)
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
  local config = assert(JSON:decode(conf_str))
  return config
end

return load_cfg
