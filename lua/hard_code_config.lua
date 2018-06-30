local ffi = require 'lua/ab_ffi'
local function l_hard_code_config(
  c_cfg
  )
  local config = {}
    -- For Database
  config.AB = {}
  config.AB.MYSQL = {}
  config.AB.MYSQL.SERVER = {}; config.AB.MYSQL.SERVER.VALUE = "127.0.0.1"
  config.AB.MYSQL.USER = {}; config.AB.MYSQL.USER.VALUE = "root"
  config.AB.MYSQL.PORT = {}; config.AB.MYSQL.PORT.VALUE = "3306"
  config.AB.MYSQL.PASSWORD = {}; config.AB.MYSQL.PASSWORD.VALUE = ""
  config.AB.MYSQL.DATABASE = {}
  config.AB.MYSQL.DATABASE.VALUE = "abdb2"
  --=====================================
  -- For Decision Tree
  -- Following is sample if you need stuff from C hard code to Lua hard cod
  c_cfg = ffi.cast("CFG_TYPE*", c_cfg)

  config.DT = {}
  config.DT.MODEL_NAME = {}
  config.DT.MODEL_NAME.VALUE = ffi.string(c_cfg.model_name)
  print(" config.DT.MODEL_NAME.VALUE = ", config.DT.MODEL_NAME.VALUE )

  return config
end
return l_hard_code_config
