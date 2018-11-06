local function get_num_features()
  assert(type(dt_features) == "table")
  return #dt_features
end
return get_num_features
