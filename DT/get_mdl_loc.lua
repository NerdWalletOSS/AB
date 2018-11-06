-- get location of model 
local function get_mdl_loc()
  assert(conf)
  assert(type(conf.DECISION_TREE_DIRECTORY) == "string")
  assert(#conf.DECISION_TREE_DIRECTORY > 0)

  assert(type(conf.MODEL_NAME) == "string")
  assert(#conf.MODEL_NAME > 0)

  return conf.DECISION_TREE_DIRECTORY, conf.MODEL_NAME
end
return get_mdl_loc
