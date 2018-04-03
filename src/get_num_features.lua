local cache = require 'cache'

local function get_nested_length(table)
	--[[
	Get the number of nested elements in a table. I.e. if a table contains a k, v pair
	with v as a table, counts the number of nested elements in that, and adds to the current count.
	]]--
	local counter = 0
	for k, v in pairs(table) do
		if type(v) == 'table' then
			counter = counter + get_nested_length(v)
		else
			counter = counter + 1
		end
	end
	return counter
end

local function get_num_features()
	local config = cache.get("table_dt_feature")
	assert(config ~= nil, 'dt_feature.lua not valid.')
	return get_nested_length(config)
end

return get_num_features