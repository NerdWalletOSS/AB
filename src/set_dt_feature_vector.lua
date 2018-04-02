local json = require 'json'
local generate_features = require 'generate_features'
local one_hot_encoding = require 'one_hot_encoding'
local get_num_features = require 'get_num_features'
local ffi = require 'ffi'

function fake_set_dt_feature_vector(str_feature_vector)
	local feature_table = json.decode(str_feature_vector)
	-- does the conversions
	feature_table = generate_features(feature_table)
	local ohe = one_hot_encoding(feature_table)
	local n_features = get_num_features()
	-- for debugging
	for k, v in pairs(ohe) do print(k, v) end
	for k, v in pairs(feature_table) do print(k, v) end
	-- end debugging
	-- init feature_vector
	local feature_vector = {}
	for i=0, n_features-1 do
		feature_vector[i] = 0
	end
	-- fill feature vector
	for k, v in pairs(ohe) do
		assert(n_features >= k, 'Index of ' ..tostring(k).. ' too big for g_dt_feature_vector, which has max ' ..tostring(n_features).. ' entries.')
		feature_vector[k - 1] = v
	end
	-- verify feature_vector
	for i=0,n_features-1 do print(i, feature_vector[i]) end
	--[[
	feature_vector = ffi.cast("float*", feature_vector)
	ffi.fill(feature_vector, ffi.sizeof("float") * n_features) -- equivalent to C's memset
	for k, v in pairs(ohe) do
		assert(n_features >= k, 'Index of ' ..tostring(k).. ' too big for g_dt_feature_vector, which has max ' ..tostring(n_features).. ' entries.')
		feature_vector[k - 1] = v
	end
	]]--
end


local function set_dt_feature_vector(str_feature_vector, feature_vector)
	local feature_table = json.decode(str_feature_vector)
	-- does the conversions
	feature_table = generate_features(feature_table)
	local ohe = one_hot_encoding(feature_table)
	local n_features = get_num_features()
	-- for debugging
	for k, v in pairs(ohe) do print(k, v) end
	-- end debugging
	feature_vector = ffi.cast("float*", feature_vector)
	ffi.fill(feature_vector, ffi.sizeof("float") * n_features) -- equivalent to C's memset
	for k, v in pairs(ohe) do
		assert(n_features >= k, 'Index of ' ..tostring(k).. ' too big for g_dt_feature_vector, which has max ' ..tostring(n_features).. ' entries.')
		feature_vector[k - 1] = v
	end
end

return set_dt_feature_vector