-- spam data set has 57 features, last one is goal to predict
-- in this case, the faature names are 0, 1, 2, ....
local dt_features = {}
for i = 1, 57 do
  dt_features[i] = "feature_" .. tostring(i + 1)
end

return dt_features
