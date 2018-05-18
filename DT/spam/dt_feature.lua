-- spam data set has 57 features, last one is goal to predict
-- in this case, the faature names are 0, 1, 2, ....
local dt_feat = {}
for i = 0, 56 do
  dt_feat[i] = i + 1
end
return dt_feat
