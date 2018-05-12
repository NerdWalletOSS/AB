local function generate_features(in_features)
  local out_features = {}
  for k, v in pairs(in_features) do 
    assertx(tonumber(k), k, ' is not a valid feature.')
    out_features[tonumber(k)] = v
  end
  return out_features
end

return generate_features
