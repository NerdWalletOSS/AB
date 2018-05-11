local ffi = require 'ab_ffi'
local function l_hard_code_config(
  c_cfg
  )
  local config = {}
    -- For Database
  config.AB = {}
  config.AB.DB = {}
  config.AB.DB.SERVER = {}; config.AB.DB.SERVER.VALUE = "localhost"
  config.AB.DB.USER = {}; config.AB.DB.USER.VALUE = "root"
  config.AB.DB.PORT = {}; config.AB.DB.PORT.VALUE = "3306"
  config.AB.DB.PASSWORD = {}; config.AB.DB.PASSWORD.VALUE = ""
  config.AB.DB.DATABASE = {}
  config.AB.DB.DATABASE.VALUE = "abdb2"
  --=====================================
  -- For Decision Tree
  -- Following is sample if you need stuff from C hard code to Lua hard cod
  --c_cfg = ffi.cast("CFG_TYPE*", c_cfg)

  config.DT = {}
  config.DT.DT_DIR = {}
  --config.DT.DT_DIR.VALUE = ffi.string(c_cfg.dt_dir)
  config.DT.DT_DIR.VALUE = 'spam'
  config.DT.DT_DIR.COMMENT = "directory where decision tree configs are"

  return config
end
return l_hard_code_config
