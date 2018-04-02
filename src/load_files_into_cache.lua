local cache = require 'cache'
local REFERRER = 'config/referrer_class_file.lua'
local DT_FEATURE = 'config/dt_feature.lua'
local POSTAL_CD_FEATURES = 'config/postal_cd_features.lua'

function load_files_into_cache()
	local status, isn, mvc, rd_sm, rd_search = pcall(dofile, REFERRER) -- keys should be the items itself
	assert(status, 'Referrer Class file: ' .. REFERRER .. ' not valid Lua file.')
	cache.put("isn", isn)
	cache.put("mvc", mvc)
	cache.put("rd_sm", rd_sm)
	cache.put("rd_search", rd_search)

	local status, postal_cd_features = pcall(dofile, POSTAL_CD_FEATURES) 
	assert(status, 'Postal Code mapping config file ' .. POSTAL_CD_FEATURES .. 'having issue.')
	cache.put("postal_cd_features", postal_cd_features)

	local status, dt_feature = pcall(dofile, DT_FEATURE)
	assert(status, 'DT Feature mapping config file ' .. DT_FEATURE .. 'having issue.')
	cache.put("dt_feature", dt_feature)
end

return load_files_into_cache