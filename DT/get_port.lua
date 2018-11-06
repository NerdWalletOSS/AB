-- get port
local function get_port()
  assert(DT)
  assert(DT.conf)
  assert(type(DT.conf.PORT) == "number")
  assert(#DT.conf.DECISION_TREE_DIRECTORY > 0)
  return DT.conf.PORT
end
return get_port
