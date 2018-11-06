local JSON = require 'lua/JSON'
-- get model meta 
local function get_mdl_meta()
  return JSON:encode(mdl_meta)
end
return get_mdl_meta
