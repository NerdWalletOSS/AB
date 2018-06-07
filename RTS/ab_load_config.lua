local assertx = require 'lua/assertx'
local cache = require 'lua/cache'
local consts = require 'lua/ab_consts'
local ffi = require 'lua/ab_ffi'
local JSON = require 'lua/JSON'
local sql = require 'lua/sql'
-- local dbg = require 'debugger'

local function is_present(v_table)
  if v_table == nil then
    return false
  else
    return true
  end
end

local function is_modified(a, b, prev)
  if prev == consts.TRUE then
    return consts.TRUE
  end
  if a == b then
    return consts.FALSE
  else
    return consts.TRUE
  end
end


local function update_string_field(field, g_cfg_value, is_updated, min, max)
  if is_updated ~= consts.TRUE then
    is_updated = consts.FALSE
  end
  min = min or 0
  max = max or consts.AB_MAX_LEN_ARGS
  if is_present(field) then
    local val = field.VALUE
    is_updated = is_modified(ffi.string(g_cfg_value) ,val, is_updated)
    assertx(#val >= min and #val <= max, "Invalid length. Length is ", #val, " for ", val)
    ffi.copy(g_cfg_value, val)
  end

  return is_updated
end

local function update_file_field(field, g_cfg_value, is_updated, min, max)
  if is_updated ~= consts.TRUE then
    is_updated = consts.FALSE
  end
  min = min or 1
  max = max or consts.AB_MAX_LEN_FILE_NAME
  -- TODO check if file exists
  return update_string_field(field, g_cfg_value, is_updated, min, max)
end

local function update_number_field(field, g_cfg_value, is_updated, min, max)
  if is_updated ~= consts.TRUE then
    is_updated = consts.FALSE
  end
  min = min or 0
  max = max or 2^32

  if is_present(field) then
    local val = assert(tonumber(field.VALUE), "must be valid number")
    assert(val >= min and val <= max, "Value must be in the valid range of min ", min, " and max ", max)
    is_updated = is_modified(g_cfg_value ,val, is_updated)
    g_cfg_value = val
  end
  return is_updated, g_cfg_value
end



local function file_exists(name)
  local f=io.open(name,"r")
  if f~=nil then
    io.close(f)
    return true
  else
    return false
  end
end
local function get_value_from_bool(x)
  if x then
    return 1
  else
    return 0
  end
end

local function update_config(c_struct, config)
  local is_updated = consts.FALSE
  is_updated, c_struct.port = update_number_field(config.PORT, c_struct.port, is_updated, 0,
  2^16 - 1)
  is_updated = update_string_field(config.SERVER, c_struct.server, is_updated,
  1, consts.AB_MAX_LEN_SERVER_NAME)
  is_updated = update_string_field(config.URL, c_struct.url, is_updated, 1,
  consts.AB_MAX_LEN_URL)
  is_updated = update_string_field(config.HEALTH_URL, c_struct.health_url,
  is_updated, 1, consts.AB_MAX_LEN_URL)
  return is_updated
end

local load_cfg = {}

function load_cfg.db_connect(mysql)
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
  -- print(host, user, pass, db, port)
  -- TODO P0
  local conn = sql:connect(host, user, pass, db, port)
  return conn
end

function load_cfg.load_db_data(config)
  local conn = load_cfg.db_connect(config.MYSQL)
  -- TODO execute the sql
  local res = conn:query('SELECT * FROM device ORDER BY id ASC;')
  table.sort(res, function(a,b) return a.id < b.id end)
  local devs = {}
  for index, entry in ipairs(res) do
    devs[entry.id] = entry.name
  end
  cache.put("devices", devs)
end

local function update_rts_configs(g_cfg, config)
  local c_struct = g_cfg[0]
  local is_updated = consts.FALSE
  is_updated,  c_struct.port = update_number_field(config.PORT, c_struct.port, is_updated, 0,
  2^16-1)
  if is_present(config.VERBOSE) then
    local verbose = -1
    if config.VERBOSE.VALUE:lower() == "false" then
      is_updated = is_modified(get_value_from_bool(g_cfg[0].verbose), consts.FALSE, is_updated)
      g_cfg[0].verbose = consts.FALSE
    elseif config.VERBOSE.VALUE:lower() == "true" then
      is_updated = is_modified(get_value_from_bool(g_cfg[0].verbose), consts.TRUE, is_updated)
      g_cfg[0].verbose = consts.TRUE
    else
      error("VERBOSE can only be true or false")
    end
  end
  is_updated, c_struct.sz_log_q = update_number_field(config.SZ_LOG_Q, c_struct.sz_log_q, is_updated, 0,
  2^32-1)
  is_updated, c_struct.num_post_retries = update_number_field(config.LOGGER.NUM_POST_RETRIES,
  c_struct.num_post_retries, is_updated, 0, 2^32-1)
  is_updated = update_string_field(config.DEFAULT_URL,
  c_struct.default_url, is_updated, 1, consts.AB_MAX_LEN_REDIRECT_URL)
  is_updated, c_struct.max_len_uuid = update_number_field(config.SZ_UUID_HT,
  c_struct.max_len_uuid, is_updated, 1, 2^32 -1)
  is_updated, c_struct.xy_guid = update_number_field(config.XY_GUID,
  c_struct.xy_guid, is_updated, 0, 2^32 -1)


  load_cfg.load_db_data(config)
  return is_updated
end

-- local function update_ml_configs(g_cfg, config)
--   local is_updated = consts.FALSE
--   local c_struct = g_cfg[0]
--
--   is_updated = update_file_field(config.DT_DIR, c_struct.dt_dir, is_updated)
--   -- is_updated = update_file_field(config.DT_BIN_FILE, c_struct.dt_file, is_updated)
--   -- is_updated = update_file_field(config.RF_BIN_FILE, c_struct.rf_file, is_updated)
--   -- is_updated = update_file_field(config.MDL_BIN_FILE, c_struct.mdl_file, is_updated)
--   is_updated = update_file_field(config.MMDB_FILE, c_struct.mmdb_file, is_updated)
--
--   return is_updated
-- end

local function update_ua_configs(g_cfg, config)
  local is_updated = consts.FALSE
  local c_struct = g_cfg[0]

  is_updated = update_file_field(config.UA_TO_DEV_MAP_FILE, c_struct.ua_to_dev_map_file, is_updated)
  is_updated = update_file_field(config.DT_BIN_FILE, c_struct.justin_cat_file, is_updated)
  is_updated = update_file_field(config.RF_BIN_FILE, c_struct.os_file, is_updated)
  is_updated = update_file_field(config.MDL_BIN_FILE, c_struct.browser_file, is_updated)
  is_updated = update_file_field(config.MMDB_FILE, c_struct.device_type_file, is_updated)

  return is_updated
end

local function update_kafka_configs(g_cfg, conf)
  local is_updated = consts.FALSE
  if conf == nil then return is_updated end
  local kafka = g_cfg[0].kafka
  is_updated = update_string_field(conf.BROKERS, kafka.brokers, is_updated, 1, consts.AB_MAX_LEN_SERVER_NAME)
  is_updated = update_string_field(conf.TOPIC, kafka.topic, is_updated, 1, consts.AB_MAX_LEN_KAFKA_TOPIC)
  is_updated = update_string_field(conf.QUEUE_SIZE, kafka.queue_size, is_updated, 1, consts.AB_MAX_LEN_KAFKA_QUEUE_SIZE)
  is_updated = update_string_field(conf.RETRIES, kafka.retries, is_updated, 1, consts.AB_MAX_LEN_KAFKA_RETRIES)
  is_updated = update_string_field(conf.MAX_BUFFERING_TIME, kafka.max_buffering_time, is_updated, 1, consts.AB_MAX_LEN_BUF_TIME)
  return is_updated
end

function load_cfg.load_config(
  config_file,
  g_cfg,  --- C data structure
  has_changed
  )

  local file = assert(io.open(ffi.string(config_file), 'r'), "Invalid filename given")
  local conf_str = file:read('*a')
  file:close()
  local config = JSON:decode(conf_str)

  assert(g_cfg ~= ffi.NULL, "Config should not be null")
  g_cfg = ffi.cast("CFG_TYPE*", g_cfg)
  -- Has changed in an array of chars - where
  -- pos 0 = RTS
  -- pos 1 = logger
  -- pos 2 = ss
  -- pos 3 = statsd
  has_changed = ffi.cast("unsigned char*", has_changed)
  ffi.fill(has_changed, ffi.sizeof("unsigned char")*7)
  -- The update functions are responsible for setting g_cfg
  has_changed[0] = update_rts_configs(g_cfg, config.AB)
  has_changed[1] = update_config(g_cfg[0].logger, config.AB.LOGGER)
  has_changed[2] = update_config(g_cfg[0].ss, config.AB.SESSION_SERVICE)
  has_changed[3] = update_config(g_cfg[0].statsd, config.AB.STATSD)
  has_changed[4] = update_config(g_cfg[0].webapp, config.AB.WEBAPP)
  -- As an example, if we wanted database info on C side, we would do
  -- has_changed[... = update_db(g_cfg, config.AB.DB)

  has_changed[5] = update_ua_configs(g_cfg, config.AB)
  -- has_changed[6] = update_ml_configs(g_cfg, config.AB)
  has_changed[6] = update_kafka_configs(g_cfg, config.AB.KAFKA)
  return config
  -- dbg()
end

return load_cfg
