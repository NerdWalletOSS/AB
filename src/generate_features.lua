local postal_cd_to_vars = require 'postal_cd_to_vars'
local time_to_vars = require 'time_to_vars'
local get_referrer_type_cd = require 'get_referrer_type_cd'

local function generate_features(feature_table)
	assert(feature_table['country'] == 'US', 'Country of origin is ' .. tostring(feature_table['country']) .. ', not "US".')
	local rtc = get_referrer_type_cd(
		feature_table['utm_med'], feature_table['utm_src'], 
		feature_table['utm_camp'], feature_table['host'], 
		feature_table['domain'])
	local pc = postal_cd_to_vars(feature_table['postal_cd'])
	local t = time_to_vars(feature_table['timestamp'],
		feature_table['timezone'])
	for k, v in pairs(pc) do feature_table[k] = v end
	for k, v in pairs(t) do feature_table[k] = v end
	feature_table['referral_type_cd'] = rtc
	return feature_table
end

return generate_features
