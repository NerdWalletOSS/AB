local cache              = require 'lua/cache'
local assertx            = require 'lua/assertx'
local x_load_config      = require 'DT/dt_load_config'
local x_hard_code_config = require 'lua/hard_code_config'
local x_update_config    = require 'DT/dt_update_config'
local make_feature_vec   = require 'DT/lua/make_feature_vector'
local get_num_feat       = require 'DT/lua/get_num_features'
local post_proc          = require 'DT/lua/post_proc_preds'
local get_model_meta       = require 'DT/lua/get_mdl_meta'


function load_config(...)
  cache.put("config", x_load_config(...))
end

function hard_code_config(...)
  cache.put("config", x_hard_code_config(...))
end

function update_config(...)
  x_update_config(...)
end

function make_feature_vector(...)
  return make_feature_vec(...)
end

function get_num_features(...)
  return get_num_feat(...)
end

function post_proc_preds(...)
  return post_proc(...)
end

function get_mdl_meta(...)
  return get_model_meta(...)
end
