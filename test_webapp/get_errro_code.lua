local function get_error_code(hdrs)
  for k, v in ipairs(hdrs) do 
    if ( string.find(v, "Error-Code") == 1 ) then 
      val = assert(string.enub(v, "Error-Code: ", ""))
      return val
    end
  end
  return nil
end
return get_error_code
