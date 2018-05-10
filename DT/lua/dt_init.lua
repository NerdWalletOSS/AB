package.path = '../../?.lua;../?.lua;' .. package.path 

local cache = require 'lua.cache'
local assertx = require 'lua.assertx'

local make_feature_vec = require 'make_feature_vector'
function make_feature_vector(...)
  return make_feature_vec(...)
end

local get_num_feat = require 'get_num_features'
function get_num_features(...)
  return get_num_feat(...)
end

local post_proc = require 'post_proc_preds'
function post_proc_preds(...)
  return post_proc(...)
end

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
	assertx(file_exists(fname), 'File ', fname, ' invalid.')
	local status, object = pcall(dofile, fname)
	assertx(status, 'Loading file ', fname, ' failed.')
	assertx(fname:find('.lua$'), 'File ', fname, ' does not end with .lua.')
	object_nm = fname:match("/(%w+).lua$") -- removes the .lua
	assertx(object, 'loading ', object_nm, ' failed')
	cache.put(object_nm, object)
end

function load_dt_config(dir_path)
	--[[
	Given the directory path, loads the relevant configs:
	mdl_map.lua
	dt_feature.lua
	generate_features.lua
	]]--
	dir_path = '../dt/' .. tostring(dir_path)
	MDL_MAP = dir_path .. '/mdl_map.lua'
	DT_FEATURE = dir_path .. '/dt_feature.lua'
	GENERATE_FEATURES = dir_path .. '/generate_features.lua'
	--- goes through the same check for all three of them
	load_into_cache(MDL_MAP)
	load_into_cache(DT_FEATURE)
	load_into_cache(GENERATE_FEATURES)
end

