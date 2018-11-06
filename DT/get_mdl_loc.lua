-- get location of model 
local function get_mdl_loc()
  assert(DT)
  assert(DT.conf)
  assert(type(DT.conf.DECISION_TREE_DIRECTORY) == "string")
  assert(#DT.conf.DECISION_TREE_DIRECTORY > 0)

  assert(type(DT.conf.MODEL_NAME) == "string")
  assert(#DT.conf.MODEL_NAME > 0)

  return DT.conf.DECISION_TREE_DIRECTORY, DT.conf.MODEL_NAME
end
return get_mdl_loc
