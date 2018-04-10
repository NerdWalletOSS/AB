local assertx = require 'assertx'
local cache = require 'cache'
local consts = require 'ab_consts'
local ffi = require 'ab_ffi'
local json = require 'json'
local sql = require 'sql'
-- local dbg = require 'debugger'

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

local function update_config(c_struct, config)
  local is_updated = consts.FALSE
  local port = tonumber(config.PORT.VALUE)
  is_updated = is_modified(c_struct.port ,port, is_updated)
  assertx(port >=0 and port <= 2^16, "Port number must be in the valid range", port)
  c_struct.port = port

  local server = config.SERVER.VALUE
  is_updated = is_modified(ffi.string(c_struct.server),server, is_updated)
  assertx(#server > 0 and #server <= consts.AB_MAX_LEN_SERVER_NAME, "server name length  must be in the valid range", server)
  ffi.copy(c_struct.server, server)

  -- TODO seems like there is no url for statsd
  if config.URL ~= nil then
    local url = config.URL.VALUE
    is_updated = is_modified(ffi.string(c_struct.url), url)
    assertx(#url > 0 and #url <= consts.AB_MAX_LEN_URL, "url length  must be in the valid range", url)
    ffi.copy(c_struct.url, url)
  end

  if config.HEALTH_URL ~= nil then
    local health_url = config.HEALTH_URL.VALUE
    is_updated = is_modified(ffi.string(c_struct.health_url), health_url, is_updated)
    assertx(#health_url > 0 and #health_url <= consts.AB_MAX_LEN_URL, "url length  must be in the valid range", url)
    ffi.copy(c_struct.health_url, health_url)
  end

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
  assert(port ~= nil and port >= 0 and port < 2^16, "Mysql entry must have a valid port")
  local db = mysql.DATABASE.VALUE
  assert(db ~= nil and type(db) == "string" and #db > 0, "Mysql entry must have a valid database")
  -- print(host, user, pass, db, port)
  local conn = sql:connect(host, user, pass, db, port)
  return conn
end

local function load_db_data(g_conf, config, is_updated)
  local conn = load_cfg.db_connect(config.MYSQL)
  -- TODO execute the sql
  local res = conn:query('SELECT * FROM device ORDER BY id ASC;')
  table.sort(res, function(a,b) return a.id < b.id end)
  local devs = {}
  for index, entry in ipairs(res) do
    devs[entry.id] = entry.name
  end
  cache.put("devices", devs)
  return is_updated
end

local function update_rts_configs(g_conf, config)
  is_updated = consts.FALSE
  local port = assert(tonumber(config.PORT.VALUE), "must be a valid port number")
  assert(port ~= nil and port >= 0 and port < 2^16, "RTS entry must have a valid port")
  is_updated = is_modified(g_conf[0].port, port, is_updated)
  g_conf[0].port = port

  local verbose = -1
  if config.VERBOSE.VALUE:lower() == "false" then
    is_updated = is_modified(get_value_from_bool(g_conf[0].verbose), consts.FALSE, is_updated)
    g_conf[0].verbose = consts.FALSE
  elseif config.VERBOSE.VALUE:lower() == "true" then
    is_updated = is_modified(get_value_from_bool(g_conf[0].verbose), consts.TRUE, is_updated)
    g_conf[0].verbose = consts.TRUE
  else
    error("VERBOSE can only be true or false")
  end

  local log_size = assert(tonumber(config.LOGGER.SZ_LOG_Q.VALUE), "must be a valid queue size")
  assert(log_size >=0 and log_size <= 2^32, "The log size must be a valid number")
  is_updated = is_modified(g_conf[0].sz_log_q, log_size, is_updated)
  g_conf[0].sz_log_q = log_size

  local num_retries = assert(tonumber(config.LOGGER.NUM_POST_RETRIES.VALUE), "Must be  valid number for max retries")
  assert(num_retries >=0 and num_retries <= 2^32, "Number or retries must be a valid value") -- TODO chat with Ramesh why not uint32_t
  is_updated = is_modified(g_conf[0].num_post_retries, num_retries, is_updated)
  g_conf[0].num_post_retries = num_retries

  local default_url = config.DEFAULT_URL.VALUE
  assert(#default_url > 0 and #default_url <= consts.AB_MAX_LEN_REDIRECT_URL, "The length of the redirect url is out of bounds")
  is_updated= is_modified(ffi.string(g_conf[0].default_url), default_url, is_updated)
  ffi.copy(g_conf[0].default_url, default_url)

  local uuid_len = assert(tonumber(config.SZ_UUID_HT.VALUE), "must be valid number")
  assert(uuid_len > 0 and uuid_len <= 2^32, "Value must be in the valid range")
  is_updated = is_modified(g_conf[0].uuid_len, uuid_len, is_updated)
  g_conf[0].uuid_len = uuid_len

  local ua_dev_file = config.UA_TO_DEV_MAP_FILE.VALUE
  assert( #ua_dev_file > 0 and #ua_dev_file <= consts.AB_MAX_LEN_FILE_NAME, "Max file len exceeded")
  is_updated = is_modified(ffi.string(g_conf[0].ua_to_dev_map_file), ua_dev_file, is_updated)
  ffi.copy(g_conf[0].ua_to_dev_map_file, ua_dev_file)

  is_updated = load_db_data(g_conf, config, is_updated)
  return is_updated
end

function load_cfg.load_transform_features(file_path) -- TODO pending test entries
  local file = assert(io.open(file_path, 'r'), "Invalid filename given")
  local conf_str = file:read('*a')
  file:close()
  local config = json.decode(cfg_str)
  config = config.AB

  --=============================================
  if ( ( config.POSTAL_CD_FEATURES ) and
    ( config.POSTAL_CD_FEATURES ~= "" ) ) then
    assert(file_exists(config.POSTAL_CD_FEATURES.VALUE))
    local status, g_postal_cd_features = pcall(dofile, config.POSTAL_CD_FEATURES.VALUE)
    assert(status, 'POSTAL_CD_FEATURES loading failed.')
    cache.put("g_postal_cd_features", g_postal_cd_features)
  end
  --=============================================

  if ( ( config.DT_FEATURE_FILE ) and
    ( config.DT_FEATURE_FILE ~= "" ) ) then
    assert(file_exists(config.DT_FEATURE_FILE.VALUE))
    local status, g_dt_feature = pcall(dofile, config.DT_FEATURE_FILE.VALUE)
    assert(status, 'DT_FEATURE_FILE loading failed.')
    cache.put("g_dt_feature", g_dt_feature)
  end
  --=============================================

  if ( ( config.REFERER_CLASS_FILE ) and
    ( config.REFERER_CLASS_FILE ~= "" ) ) then
    assert(file_exists(config.REFERER_CLASS_FILE.VALUE))
    local status, referer_class_tables = pcall(dofile, config.REFERER_CLASS_FILE.VALUE)
    assert(status, 'REFERER_CLASS_FILE loading failed')
    assert(referer_class_tables, 'loading referer_class_tables failed')
    assert(referer_class_tables["isn"], 'loading g_table_isn failed')
    assert(referer_class_tables["mvc"], 'loading g_table_mvc failed')
    assert(referer_class_tables["rd_sm"], 'loading g_table_rd_sm failed')
    assert(referer_class_tables["rd_search"], 'loading g_table_rd_search failed')
    cache.put("g_table_isn", referer_class_tables["isn"])
    cache.put("g_table_mvc", referer_class_tables["mvc"])
    cache.put("g_table_rd_sm", referer_class_tables["rd_sm"])
    cache.put("g_table_rd_search", referer_class_tables["rd_search"])
  end
  --=============================================

  if ( ( config.CCID_MAPPING ) and
    ( config.CCID_MAPPING ~= "" ) ) then
    assert(file_exists(config.CCID_MAPPING.VALUE))
    local status, g_ccid_mapping = pcall(dofile, config.CCID_MAPPING.VALUE)
    assert(status, 'CCID_MAPPING loading failed.')
    cache.put("g_ccid_mapping", g_ccid_mapping)
  end
  --=============================================

end

function load_cfg.load_config(conf_str, g_conf, has_changed)
  local config = json.decode(conf_str)

  g_conf = ffi.cast("CFG_TYPE*", g_conf)
  assert(g_conf ~= ffi.NULL, "Config should not be null")
  -- Has changed in an array of chars - where
  -- pos 0 = RTS
  -- pos 1 = logger
  -- pos 2 = ss
  -- pos 3 = statsd
  has_changed = ffi.cast("unsigned char*", has_changed)
  has_changed[0] = update_rts_configs(g_conf, config.AB)
  has_changed[1] = update_config(g_conf[0].logger, config.AB.LOGGER)
  has_changed[2] = update_config(g_conf[0].ss, config.AB.SESSION_SERVICE)
  has_changed[3] = update_config(g_conf[0].statsd, config.AB.STATSD)
  cache.put("config", config)
  -- dbg()
end

function load_cfg.load_config_from_file(g_conf, has_changed, file_path)
  local file = assert(io.open(file_path, 'r'), "Invalid filename given")
  local conf_str = file:read('*a')
  file:close()
  return load_cfg.load_config(conf_str, g_conf, has_changed)
end

return load_cfg
