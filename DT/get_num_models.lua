local function get_num_models()
  assert(type(mdl_map) == "table")
  return #mdl_map
end
return get_num_models
