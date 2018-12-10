local load_model = require 'DT/load_model'

local function proc_config_file(
  config_file
  )
  print(config_file)
  conf = assert(loadfile(config_file)()) -- global
  for k, v in pairs(conf) do print(k, v) end
  local status = load_model(conf.DECISION_TREE_DIRECTORY)
  if not status then return false end 
  return true
end
return proc_config_file
