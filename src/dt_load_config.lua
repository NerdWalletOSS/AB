local assertx = require 'assertx'
local json = require 'json'

function load_cfg.load_config(
  config_file,
  g_cfg,  --- C data structure
  has_changed
  )
  local file = assert(io.open(config_file, 'r'), "Invalid filename given")
  local conf_str = file:read('*a')
  file:close()
  local config = json.decode(conf_str)
  -- If you want to update g_cfg, do this before you return
  -- as an example
  -- update_xxx_configs(g_cfg, config.DT)

  return config
end
