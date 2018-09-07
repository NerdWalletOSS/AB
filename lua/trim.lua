local function trim(s)
  assert(s, "No argument to trim")
  assert(type(s) == "string", "arg to trim not a string")
    return (s:gsub("^%s*(.-)%s*$", "%1"))
end
return trim
