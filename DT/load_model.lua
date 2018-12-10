local function load_model(
  model_dir
  )
  --===========================================
  dt_features = require(model_dir .. "/dt_features")
  assert(type(dt_features) == "table")
  assert(#dt_features > 0)
  dt_feature_to_idx = {}
  for k, v in ipairs(dt_features) do
    dt_feature_to_idx[v] = k
  end
  --===========================================
  local mdl_meta_file = model_dir .. "/mdl_meta"
  mdl_meta = {}
  mdl_meta = require(mdl_meta_file)
  assert(type(mdl_meta) == "table")
  --===========================================
  local mdl_map_file = model_dir .. "/mdl_map"
  mdl_map = {}
  mdl_map = require(mdl_map_file)
  assert(type(mdl_map) == "table")
  return true
end
return load_model

--===========================================
-- for k, v in pairs(mdl_meta) do print(k, v) end 
--===========================================
