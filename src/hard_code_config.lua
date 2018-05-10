local ffi = require 'ab_ffi'
local function l_hard_code_config(
  c_cfg
  )
  local config = {
    -- For Database
    config.AB.DB = {}
    config.AB.DB.HOST = "localhost"
    config.AB.DB.USER = "root"
    config.AB.DB.PASS = ""
    config.AB.DB.NAME = "abdb2"
    --=====================================
    -- For Decision Tree
    config.DT.DT_DIR = {}
    config.DT.DT_DIR.VALUE = "../DT/spam/"
    config.DT.DT_DIR.COMMENT = "directory where decision tree configs are"
  }
  -- Following is sample if you need stuff from C hard code to Lua hard cod
  config.DT.MMDB_FILE = {}
  config.DT.MMDB_FILE.VALUE = ffi.string(c_cfg.mmdb_file)
  config.DT.MMDB_FILE.COMMENT = "maxmind database file"
  return config
end
return l_hard_code_config
