-- get port
local function get_port()
  assert(conf)
  if ( type(conf.PORT) == "number" )  then
    return conf.PORT
  else
    return 0
  end
end
return get_port
