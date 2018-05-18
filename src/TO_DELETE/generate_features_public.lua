local function generate_features_public(in_features)
	out_features = {}
	for i=0, 56 do
		local var = assertx(in_features[i] or 
			in_features[tostring(i)], 
			'Feature ', tostring(i), ' missing.')
		var = assertx(tonumber(var), 'Variable ', tostring(var), 
			' is not a number.')
		out_features[i] = var
	end
	return out_features
end


return generate_features_public