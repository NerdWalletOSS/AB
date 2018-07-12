local body = ""
local function get_body (str)
  body = str
end
local function reset_body ()
  body = ""
end
local function return_body ()
  return body
end
return { get_body, reset_body, return_body}
