-- In general, we need to return a Lua table indexed as 1, 2, ... n
-- where n is the number of features and each value is a string
-- In this case, spam data set has 57 features, last one being goal 
-- The feature names are feature_0, feature_1, ...
local dt_features = {}
for i = 1, 57 do
  dt_features[i] = "feature_" .. tostring(i-1)
end
return dt_features
