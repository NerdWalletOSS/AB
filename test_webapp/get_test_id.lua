local function get_test_id(hdrs)
  local test_id 
  for k, v in ipairs(hdrs) do 
    print(k, v)
    if ( string.find(v, "TestID") == 1 ) then 
      test_id = assert(string.gsub(v, "TestID: ", ""))
      test_id = assert(tonumber(test_id))
    end
  end
  assert(test_id)
  assert(test_id > 0)
  return test_id
end
return get_test_id
