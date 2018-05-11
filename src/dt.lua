local cache = require 'cache'
local assertx = require 'assertx'
local x_load_config = require 'dt_load_config'
local x_hard_code_config = require 'hard_code_config'
local x_update_config = require 'dt_update_config'
package.path = '../?.lua;' .. package.path 
local make_feature_vec = require 'DT.lua.make_feature_vector'
local get_num_feat = require 'DT.lua.get_num_features'
local post_proc = require 'DT.lua.post_proc_preds'

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
