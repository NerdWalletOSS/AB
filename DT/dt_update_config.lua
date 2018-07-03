local assertx = require 'lua/assertx'
local cache   = require 'lua/cache'

local function file_exists(name)
  local f = io.open(name,"r")
  if f then
    io.close(f)
    return true
  else
    return false
  end
end

local function load_into_cache(fname)
  print("Lua: load_into_cache ", fname)
  local object = assertx(require(fname), "File not found " .. fname)
  -- because we are pcalling from C
  object_nm = fname:match("/([A-Za-z0-9_-]+)$") -- gets file name
  assertx(object, 'loading ', object_nm, ' failed')
  cache.put(object_nm, object)
end

local function update_config()
  --[[
    Function to update config.
    This is where each of the functions required for
    running a specific model is loaded into the cache.
  ]]--
  print("Lua: update_config()");
  local config = assert(cache.get("config"))
  assert(config.DT, "DT hard-coded values in C not in Lua cache.")
  local model_name = config.DT.MODEL_NAME.VALUE
  if ( ( model_name ) and ( #model_name > 0 ) ) then 
    -- existence of dt_dir checked by C 
    DT_FEATURE        = "DT/" .. model_name .. "/dt_feature"
    GENERATE_FEATURES = "DT/" .. model_name .. "/generate_features"
    MDL_MAP           = "DT/" .. model_name .. "/mdl_map"
    MDL_META          = "DT/" .. model_name .. "/mdl_meta"
    load_into_cache(DT_FEATURE)
    load_into_cache(MDL_MAP)
    load_into_cache(MDL_META)
    load_into_cache(GENERATE_FEATURES)
  end
end

return update_config
