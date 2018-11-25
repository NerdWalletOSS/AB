-- get port
local function get_port()
  assert(conf)
  assert(type(conf.FOREST_TYPE) == "string")
  return conf.FOREST_TYPE
end
return get_port
