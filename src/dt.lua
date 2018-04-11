local load_cfg = require 'load_config'

function load_config(...)
  load_cfg.load_transform_features(...)
end

local xxx = require 'make_feature_vector'
function make_feature_vector(...)
  return xxx(...)
end

local xxx = require 'get_num_features'
function get_num_features(...)
  return xxx(...)
end

local xxx = require 'get_n_cards'
function get_n_cards(...)
  return xxx(...)
end

local xxx = require 'post_proc_preds'
function post_proc_preds(...)
  return xxx(...)
end


