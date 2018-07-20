local assertx = require 'lua/assertx'
local ffi     = require 'lua/ab_ffi'

local function isfile(name)
  local f = io.open(name,"r")
  if f then
    io.close(f)
    return true
  else
    return false
  end
end

local function update_config(
  cfg
  )
  --[[
    Function to update config.
    This is where each of the functions required for
    running a specific model is loaded into the cache.
  ]]--
  local cfg = ffi.cast("CFG_TYPE *", cfg)
  local model_name = ffi.string(cfg[0].model_name)
  assert(model_name)
  -- existence of dt_dir checked by C 
  local dt_feature        = "DT/" .. model_name .. "/dt_feature"
  local generate_features = "DT/" .. model_name .. "/generate_features"
  local mdl_map           = "DT/" .. model_name .. "/mdl_map"
  local mdl_meta          = "DT/" .. model_name .. "/mdl_meta"
  isfile(dt_feature .. ".lua")
    
    -- load_into_cache(DT_FEATURE)
    -- load_into_cache(MDL_MAP)
    -- load_into_cache(MDL_META)
    -- load_into_cache(GENERATE_FEATURES)
end

return update_config
