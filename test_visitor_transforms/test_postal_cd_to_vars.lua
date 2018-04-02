describe("Testing postal_cd_to_vars", function()
	package.path = package.path .. ';../src/?.lua'
	local postal_cd_to_vars = require 'postal_cd_to_vars'
	local postal_cd_features = dofile('../config/postal_cd_features.lua')
	local cache = require 'cache'
	cache.put("postal_cd_features", postal_cd_features)
	local good_postal_codes = {}
	for k, _ in pairs(postal_cd_features) do 
		good_postal_codes[#good_postal_codes+1] = k 
	end
	local bad_postal_codes = {}
	for i=1,100000 do
		if postal_cd_features[i] == nil then
		bad_postal_codes[#bad_postal_codes+1] = i
		end
	end


	it("should be able to give out valid outputs to 100 good randomly generated inputs (postal code as numeric)", function()
		for i=1,1000 do
			pc = good_postal_codes[math.random(#good_postal_codes)]
			assert.truthy(postal_cd_to_vars(pc))
		end
	end)

	it("should be able to give out valid outputs to 100 good randomly generated inputs (postal code as string)", function()
		for i=1,1000 do
			pc = tostring(good_postal_codes[math.random(#good_postal_codes)])
			assert.truthy(postal_cd_to_vars(pc))
		end
	end)

	it("should be able to fail when 100 randomly generated invalid inputs are provided", function()
		for i=1,1000 do
			pc = tostring(bad_postal_codes[math.random(#bad_postal_codes)])
			assert.is_false(pcall(postal_cd_to_vars, pc))
		end
	end)

	it("should fail when postal code is out of range", function()
	-- deep check comparisons!
		assert.is_false(pcall(postal_cd_to_vars, -1))
		assert.is_false(pcall(postal_cd_to_vars, 100000))
	end)

	it("should fail when postal code is not a number", function()
	-- deep check comparisons!
		assert.is_false(pcall(postal_cd_to_vars, 'SW1A 1AA'))
	end)
end)