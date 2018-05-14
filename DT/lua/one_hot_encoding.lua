--[[
Read through the config file (dt_feature), which should have
been saved as a Lua file, and one-hot-encodes it
using the recipes in it.

dt_feature is structured as a {k: v or {vk: vv}} table,
i.e. there is one level of nesting for
categorical variables.
k is the variable name. Here it is var_nm
encoding_rule can either be v:
	v is the index/position (1-indexed) of the non-categorical variable
OR {vk: vv}:
	vk is the possible value of the categorical variable
	vv is the index/position (1-indexed) of the categorical variable for
	   that particular value (vk).

Therefore:
1. If feature_table contains key-value pairs that are not
   in the variable list, this handily skips over it and ignores it.
2. This will suddenly fail if any of the non-categorical
   variables are not numeric.
3. If there is a value within the categorical variables that
   do not exist in dt_feature, none of the values will be encoded.
]]--
local cache = require 'lua.cache'
local assertx = require 'lua.assertx'

local function one_hot_encoding(feature_table)
	--[[
	This step does
	1. the checking to make sure all inputs of the model
	   (hard-coded in dt_feature) are present in the feature table
	2. the encoding; for each variable, the encoding rule is retrieved.
	   index refers to the index of the final array (1-indexed)
     final_value is the value that is to be placed in there:
       - for categorical (i.e. if encoding_rule is a table) it is 1
       - for boolean it is either 1 or 0
       - for numeric it is the value

	  Here:
	  - var_nm is the variable name
	  - var_value is the value for that particular variable, e.g. 749 
	    or 'Tablet'
	  - final_value is the numeric value after var_value has been processed
	  - index is the index of the final vector that will be filled up with
	  	final_value
  	- not all indices will be touched; those that are not are assumed 0
	]]--
	local dt_feature = assert(cache.get("dt_feature"),
		"dt_feature not loaded.")
	local output = {}
	for var_nm, encoding_rule in pairs(dt_feature) do
		local var_value = feature_table[var_nm]
		local final_value = nil
		assertx(var_value ~= nil, 'Feature ', var_nm,
			' present in model not found in payload.')
		local index = nil
		if type(encoding_rule) == 'table' then
			index = encoding_rule[var_value]
			final_value = 1
		else -- if it's not {vk:vv} then it must be v
			assert(type(encoding_rule) == 'number',
				'dt_feature file has values not table nor number.')
			index = encoding_rule
			if type(var_value) == 'boolean' then
				final_value = var_value and 1 or 0
			else
				assertx(tonumber(var_value), 'Value of var ', var_nm,
					' is not a number: ', tostring(var_value))
				final_value = tonumber(var_value)
			end
		end
		if index then -- case when categorical value is not one of the options
			output[index] = final_value
		end
	end
	return output
end


return one_hot_encoding