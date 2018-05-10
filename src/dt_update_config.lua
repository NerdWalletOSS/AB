local cache = require 'cache'

local function update_config()
  local config = assert(cache.get("config"))
  assert(config.DT)
  local dt_dir = config.DT.DT_DIR.VALUE
  if ( ( dt_dir ) and ( #dt_dir > 0 ) ) then 
    -- existence of dt_dir checked by C 
    
    local make_feature_vec = require(dt_dir .. '/make_feature_vector')
    function make_feature_vector(...)
      return make_feature_vec(...)
    end
    
    local get_num_features = require(dt_dir .. '/get_num_features')
    function get_num_features(...)
      return get_num_feat(...)
    end
    
    local post_proc = require(dt_dir .. '/post_proc_preds')
    function post_proc_preds(...)
      return post_proc(...)
    end
  end
end
return update_config
