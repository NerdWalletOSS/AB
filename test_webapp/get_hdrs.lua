local hdrs = {}
local function trim1(s)
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end
local function get_hdrs (str)
  hdrs[#hdrs+1] = trim1(str)
end
local function reset_hdrs ()
  hdrs = {}
  hdrs[1] = "abc"
end
local function return_hdrs ()
  return hdrs
end
return { get_hdrs, reset_hdrs, return_hdrs }
