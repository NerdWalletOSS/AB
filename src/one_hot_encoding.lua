local assertx = require 'assertx'

local function one_hot_encoding(feature_table)
	--[[
	Read through the config file (ENCODING_RULES), which should have been saved as a Lua file, 
	and one-hot-encodes it using the recipes in it.

	ENCODING_RULES is structured as a {k: v or {vk: vv}} table, i.e. there is one level of nesting for
	categorical variables.
	k is the variable name.
	v is the position (1-indexed) of the non-categorical variable
	vk is the possible value of the categorical variable
	vv is the position (1-indexed) of the categorical variable

	Therefore:
	1. If feature_table contains key-value pairs that are not in the variable list, this handily skips over it and ignores it.
	2. This will suddenly fail if any of the non-categorical variables are not numeric.
	3. If there is a value within the categorical variables that do not exist in ENCODING_RULES, none of the values will be encoded.
	]]--
	assert(g_dt_feature, "g_dt_feature not loaded.")
	--local table_dt_feature = cache.get("table_dt_feature")
	local output = {}
	-- checking to make sure all inputs are present
	for var, _ in pairs(g_dt_feature) do
		assertx(feature_table[var] ~= nil, 'Feature ', var, ' present in model not found in payload.')
	end
	for var, raw_value in pairs(feature_table) do
		local index = nil
		local cat_encoding_rules = g_dt_feature[var]
		if type(cat_encoding_rules) == 'table' then
			index = cat_encoding_rules[raw_value]
			final_value = 1
		elseif type(cat_encoding_rules) == 'number' then
			index = cat_encoding_rules
			if type(raw_value) == 'boolean' then
				if raw_value == true then
					final_value = 1
				else
					final_value = 0
				end
			else
				assertx(tonumber(raw_value) or type(raw_value) == 'number', 'Value of var ', var, ' is not a number: ', tostring(raw_value))
				if type(raw_value) ~= 'number' then
					raw_value = tonumber(raw_value)
				end
				final_value = raw_value
			end
		end
		if index then -- special case when the categorical value is missing from the options
			output[index] = final_value
		end
	end
	return output
end


return one_hot_encoding