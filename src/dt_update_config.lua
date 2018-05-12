local cache = require 'cache'
package.path = '../?.lua'

local assertx = require 'lua.assertx'
local dt_cache = require 'lua.cache'

local function file_exists(name)
  local f=io.open(name,"r")
  if f~=nil then
    io.close(f)
    return true
  else
    return false
  end
end


local function load_into_cache(fname)
  assertx(file_exists(fname .. '.lua'), 'File ', fname, ' invalid.')
  local object = require (fname) -- if this crashes here it's fine
  -- because we are pcalling from C
  object_nm = fname:match("/([A-Za-z0-9_-]+)$") -- gets file name
  assertx(object, 'loading ', object_nm, ' failed')
  cache.put(object_nm, object)
end


local function update_config()
  local config = assert(cache.get("config"))
  assert(config.DT)
  local dt_dir = config.DT.DT_DIR.VALUE
  if ( ( dt_dir ) and ( #dt_dir > 0 ) ) then 
    -- existence of dt_dir checked by C 
    dir_path = '../DT/' .. tostring(dt_dir)
    MDL_MAP = dir_path .. '/mdl_map'
    DT_FEATURE = dir_path .. '/dt_feature'
    GENERATE_FEATURES = dir_path .. '/generate_features'
    --- goes through the same check for all three of them
    load_into_cache(MDL_MAP)
    load_into_cache(DT_FEATURE)
    load_into_cache(GENERATE_FEATURES)
  end
end
return update_config