local function get_error_code(hdrs)
  local error_code
  for k, v in ipairs(hdrs) do 
    local needle = 'Error%-Code:' -- REMEMBER TO ESCAPE THE HYPHEN
    if ( string.find(v, needle) == 1 ) then 
      error_code = assert(string.gsub(v, needle, ""))
      error_code = assert(tonumber(error_code))
    end
  end
  return error_code
end
return get_error_code
