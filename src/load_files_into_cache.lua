local cache = require 'cache'
local assertx = require 'assertx'
local REFERRER = 'config/referrer_class_file.lua'
local DT_FEATURE = 'config/dt_feature.lua'
local POSTAL_CD_FEATURES = 'config/postal_cd_features.lua'

function load_files_into_cache()
	local status, table_isn, table_mvc, table_rd_sm, table_rd_search = pcall(dofile, REFERRER) -- keys should be the items itself
	assertx(status, 'Referrer Class file: ', REFERRER, ' not valid Lua file.')
	cache.put("table_isn", table_isn)
	cache.put("table_mvc", table_mvc)
	cache.put("table_rd_sm", table_rd_sm)
	cache.put("table_rd_search", table_rd_search)

	local status, table_postal_cd_features = pcall(dofile, POSTAL_CD_FEATURES) 
	assertx(status, 'Postal Code mapping config file ', POSTAL_CD_FEATURES, 'having issue.')
	cache.put("table_postal_cd_features", table_postal_cd_features)

	local status, table_dt_feature = pcall(dofile, DT_FEATURE)
	assertx(status, 'DT Feature mapping config file ', DT_FEATURE, 'having issue.')
	cache.put("table_dt_feature", table_dt_feature)
end

return load_files_into_cache