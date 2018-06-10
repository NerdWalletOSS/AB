local cache            = require 'lua/cache'
local JSON             = require 'lua/JSON' 
local assertx          = require 'lua/assertx'
local one_hot_encoding = require 'DT/lua/one_hot_encoding'
local get_num_features = require 'DT/lua/get_num_features'
local ffi              = require 'ffi'

local function make_mdl_meta(
  out_buf, -- output buffer for debugging 
  sz_out_buf -- size of above
  )
  -- TODO LUMPY
  return true
end

return make_mdl_meta
