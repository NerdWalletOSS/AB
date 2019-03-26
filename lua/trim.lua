local function trim(s)
<<<<<<< HEAD
=======
  assert(s, "No argument to trim")
  assert(type(s) == "string", "arg to trim not a string")
>>>>>>> dev
    return (s:gsub("^%s*(.-)%s*$", "%1"))
end
return trim
