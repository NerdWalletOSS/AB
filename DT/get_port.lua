-- get port
local function get_port()
  assert(conf)
  assert(type(conf.PORT) == "number")
  return conf.PORT
end
return get_port
