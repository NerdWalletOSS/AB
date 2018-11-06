local function isfile(name)
  local f = io.open(name,"r")
  if f then
    io.close(f)
    return true
  else
    return false
  end
end
return isfile
