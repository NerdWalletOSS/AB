local states = { "started", "terminated", "archived" }
local test_types = { "ABTest", "XYTest" }
local is_dev_specific = { 
  "0", 
  -- "1", 
}
local channels = {
'aol',
'bing',
'cnn',
'dianomi',
'facebook',
'google',
'instagram',
'outbrain',
'pandora',
'pinterest',
'pocket',
'taboola',
'twitter',
'weather_channel',
'yahoo',
'youtube',
}
local rts_endpoints = {
"add_test",
-- "chk_db_conn",
-- "chk_test",
-- "get_config",
"list_tests",
-- "reload",
-- "reset_db",
-- "test_info",
-- "update_config",
"diagnostics",
}

local function is_in_set(
  sname, 
  sets
  )
  for _, v in pairs(sets) do
    if ( v == sname ) then return true end
  end
  return false
end


local sets = { }
sets["states"] = states
sets["test_types"]   = test_types
sets["is_dev_specific"] = is_dev_specific
sets["channels"] = channels
sets["rts_endpoints"] = rts_endpoints

local function rand_set(sname)
  assert(type(sname) == "string")
  local S = assert(sets[sname], "no set with name " .. sname)
  assert(type(S) == "table")
  local sidx = math.random(#S) 
  assert(S[sidx], "no value for index = " .. sidx .. " and set = " .. sname)
  return S[sidx]
end
return rand_set
