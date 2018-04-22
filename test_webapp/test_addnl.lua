require 'strict'
local JSON = (loadfile "JSON.lua")() -- one-time load of the routines
local cURL = require "cURL"
local ltn12 = require("ltn12") 
local plstr = require 'pl.stringx'
local plpath = require 'pl.path'
local get_test_id = require 'get_test_id'
local get_error_code = require 'get_error_code'
local test_compare = require 'test_compare'
local get_url = require 'get_url'
local reset_db = require 'reset_db'

local suite_description = [[
  These tests verfy that basic addition and modification of a test
  as permissible on Screen 1 can be performed. 
]]

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

--============================
-- START: Stuff common to all tests in this suite
local chk_url = "localhost:8080/AB/php/endpoints/endpoint_test_info.php?TestID="
curl_params = { 
  url = "localhost:8080/AB/php/endpoints/endpoint_test_basic.php",
  post       = true,
  httpheader = { "Content-Type: application/json"; },
  writefunction  = get_body,
  headerfunction = get_hdrs,
}
addnl_curl_params = { 
  url = "localhost:8080/AB/php/endpoints/endpoint_add_addnl_var_info.php",
  post       = true,
  httpheader = { "Content-Type: application/json"; },
  writefunction  = get_body,
  headerfunction = get_hdrs,
}
-- STOP: Stuff common to all tests in this suite
local tests = {}
tests.t1 = function (
  just_pr
  )
  local description = "Create a number of valid tests "
  if ( just_pr ) then print(description) return end

  local idx = 1
  local n_good = 0
  local n_bad = 0
  local prefixes  = { "good", "bad" }
  for k, prefix in pairs(prefixes) do
    print("prefix = ",  prefix)
    while true do 
      local filename = prefix .. "_addnl" .. idx .. ".lua"
      print(filename)
      if ( not plpath.isfile(filename) ) then idx = 1; break end 
      reset_db()
  
      local init_filename = "good_basic1.lua" 
      assert(plpath.isfile(init_filename) )
  
      local T = dofile(init_filename)
      hdrs = {} ; body = "" 
      curl_params.postfields = JSON:encode(T)
      local c = cURL.easy(curl_params)
      local x = c:perform()
      local test_id = assert(get_test_id(hdrs))
      assert(test_id > 0)
      local error_code = assert(get_error_code(hdrs))
      assert(error_code == 200)
      -- Check that test info is same as what you sent in
      local body_out, hdrs_out = get_url(chk_url .. test_id)
      local Tout = assert(JSON:decode(body_out))
      assert(test_compare(T, Tout))
      c:close()
      -- set and unset description and custom data
      -- select arbitrary variant_id to modify
      local Variants = Tout.Variants
      local vidx = math.random(1, #Variants)
      local variant_id = Tout.Variants[vidx]['id']
      local V = dofile(filename)
      Variants[vidx]['description'] = V.description
      Variants[vidx]['custom_data'] = V.custom_data
      Variants[vidx]['url'] = V.url
      T.id = test_id
      T.name = nil
      T.Updater = "joe"
      T.Variants = Variants
      T.VariantID = variant_id
      hdrs = {} ; body = "" 
      addnl_curl_params.postfields = JSON:encode(T)
      local c = cURL.easy(addnl_curl_params)
      local x = c:perform()
      -- for k, v in pairs(hdrs) do print(k, v) end 
      local error_code = assert(get_error_code(hdrs))
      if ( prefix == "good" ) then 
        assert(error_code == 200, "Failure on " .. filename)
        -- Check that test info is same as what you sent in
        local body_out, hdrs_out = get_url(chk_url .. test_id)
        local Tout = assert(JSON:decode(body_out))
        assert(test_compare(T, Tout))
        n_good = n_good + 1
      elseif ( prefix == "bad" ) then 
        assert(error_code == 400)
        n_bad = n_bad + 1
      else
        assert(nil, "bad prefix")
      end
      print("t1 succeeded on " .. filename)
      idx = idx + 1
    end
  end
  print("Test t1 [" .. n_good .. ", " .. n_bad .."] succeeded")
  return true
end
tests.t1() 
return tests, suite_description
