DT = {} -- this is a global

DT.conf = dofile('../dt_server/dt.conf.lua')

get_mdl_loc = require 'DT/get_mdl_loc'
get_num_features = require 'DT/get_num_features'
get_port = require 'DT/get_port'

local _, model_name = get_mdl_loc()

dt_features = require('DT/' .. model_name .. "/dt_features")
assert(type(dt_features) == "table")
assert(#dt_features > 0)
for k, v in pairs(dt_features) do print(k,v) end
