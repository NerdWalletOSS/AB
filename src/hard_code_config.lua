local ffi = require 'ab_ffi'
local function l_hard_code_config(
  c_cfg
  )
  local config = {}
    -- For Database
  config.AB = {}
  config.AB.DB = {}
  config.AB.DB.HOST = {}
  config.AB.DB.USER = {}
  config.AB.DB.PASS = {}
  config.AB.DB.NAME = {}
  config.AB.DB.HOST.VALUE = "localhost"
  config.AB.DB.USER.VALUE = "root"
  config.AB.DB.PASS.VALUE = ""
  config.AB.DB.NAME.VALUE = "abdb2"
  --=====================================
  -- For Decision Tree
  config.DT = {}
  config.DT.DT_DIR = {}
  config.DT.DT_DIR.VALUE = "../DT/spam/"
  config.DT.DT_DIR.COMMENT = "directory where decision tree configs are"

  -- Following is sample if you need stuff from C hard code to Lua hard cod
  config.DT.MMDB_FILE = {}
  c_cfg = ffi.cast("CFG_TYPE*", c_cfg)
  config.DT.MMDB_FILE.VALUE = ffi.string(c_cfg.mmdb_file)
  config.DT.MMDB_FILE.COMMENT = "maxmind database file"
  return config
end
return l_hard_code_config
