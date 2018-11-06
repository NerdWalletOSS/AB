local function set_model_name(
  model_name
  )
  assert(model_name)
  assert(type(model_name) == "string")
  assert(#model_name > 0)
  conf.MODEL_NAME = model_name
end
return set_model_name
