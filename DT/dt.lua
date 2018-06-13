local assertx               = require 'lua/assertx'
local cache                 = require 'lua/cache'
local x_get_mdl_meta        = require 'DT/lua/get_mdl_meta'
local x_get_num_features    = require 'DT/lua/get_num_features'
local x_hard_code_config    = require 'lua/hard_code_config'
local x_load_config         = require 'DT/dt_load_config'
local x_make_feature_vector = require 'DT/lua/make_feature_vector'
local x_post_proc_preds     = require 'DT/lua/post_proc_preds'
local x_update_config       = require 'DT/dt_update_config'

function get_mdl_meta(...)
  return x_get_mdl_meta(...)
end

function get_num_features(...)
  return x_get_num_features(...)
end

function hard_code_config(...)
  cache.put("config", x_hard_code_config(...))
end

function load_config(...)
  cache.put("config", x_load_config(...))
end

function make_feature_vector(...)
  return x_make_feature_vector(...)
end

function post_proc_preds(...)
  return x_post_proc_preds(...)
end

function update_config(...)
  x_update_config(...)
end