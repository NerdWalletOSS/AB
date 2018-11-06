local isfile = require 'DT/isfile'
conf = dofile('../dt_server/dt.conf.lua')

get_config       = require 'DT/get_config'
get_mdl_loc      = require 'DT/get_mdl_loc'
get_mdl_meta     = require 'DT/get_mdl_meta'
set_model_name   = require 'DT/set_model_name'
get_num_features = require 'DT/get_num_features'
get_num_models   = require 'DT/get_num_models'
make_feature_vector =  require 'DT/make_feature_vector'
get_port = require 'DT/get_port'

local _, model_name = get_mdl_loc()
--===========================================
dt_features = require('DT/' .. model_name .. "/dt_features")
assert(type(dt_features) == "table")
assert(#dt_features > 0)
dt_feature_to_idx = {}
for k, v in ipairs(dt_features) do
  dt_feature_to_idx[v] = k
end
--===========================================
local mdl_meta_file = 'DT/' .. model_name .. "/mdl_meta"
mdl_meta = {}
mdl_meta = require(mdl_meta_file)
assert(type(mdl_meta) == "table")
--===========================================
local mdl_map_file = 'DT/' .. model_name .. "/mdl_map"
mdl_map = {}
mdl_map = require(mdl_map_file)
assert(type(mdl_map) == "table")
--===========================================
-- for k, v in pairs(mdl_meta) do print(k, v) end 
