local load_cfg = require 'load_config'

function load_config(...)
  load_cfg.load_transform_features(...)
end

local make_feature_vec = require 'make_feature_vector'
function make_feature_vector(...)
  return make_feature_vec(...)
end

local get_num_feat = require 'get_num_features'
function get_num_features(...)
  return get_num_feat(...)
end

local get_n_cds = require 'get_n_cards'
function get_n_cards(...)
  return get_n_cds(...)
end

local post_proc = require 'post_proc_preds'
function post_proc_preds(...)
  return post_proc(...)
end


