--local POSTAL_CD_MAPPING = 'postal_cd_features.lua'
local cache = require 'cache'
local assertx = require 'assertx'

local function postal_code_to_vars(zip_code)
	-- expect zip_code as a string, but otherwise it's okay
	if type(zip_code) == 'string' and tonumber(zip_code) ~= nil then
		zip_code = tonumber(zip_code)
	end
	--[[
	local status, postal_dict = pcall(dofile, POSTAL_CD_MAPPING) 
	assert(status, 'Postal Code mapping config file ' .. POSTAL_CD_MAPPING .. 'having issue.')
	]]--
	local postal_dict = cache.get("postal_cd_features")
	op = postal_dict[zip_code]
	assertx(op, 'Postal code ', zip_code, ' not found in Zillow/TU database.')
	return op
end

return postal_code_to_vars