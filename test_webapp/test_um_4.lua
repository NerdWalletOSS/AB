require 'lua/str'
local JSON = require 'lua/JSON'
local curl = require 'lua/curl'
local mk_test = require 'test_webapp/mk_test'
local reset_db = require 'test_webapp/reset_db'
local get_test_id = require 'test_webapp/get_test_id'
local get_test_info = require 'test_webapp/get_test_info'
local get_error_code = require 'test_webapp/get_error_code'
local S = require 'state_change'
local states = require 'states'
--==========================
local ssurl =  -- set state URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_set_state.php"
local tburl =  -- test basic URL 
 "http://localhost:8080/AB/php/endpoints/endpoint_test_basic.php"


-- GET VARIANT STARTS. TO BE REMOVED LATER

local get_test_info = require 'test_webapp/get_test_info'
local function get_variant(test_id, vname)
  assert(test_id)
  assert(type(test_id) == "number")
  assert(vname)
  assert(type(vname) == "string")
  local V = {}
  local T = assert(get_test_info(test_id))
  local Variants = T.Variants
  for k, v in pairs(Variants) do 
    local is_found = false
    for k2, v2 in pairs(v) do 
      if ( k2 == "name" ) then 
        if ( v2 == vname ) then
          is_found = true
        end
      end
    end
    if ( is_found ) then return v end 
  end
  return nil
end

-- GET VARIANT ENDS. TO BE REMOVED LATER



-- STOP: Stuff common to all tests in this suite
local tests = {}
tests.t1 = function (
  just_pr
  )
  local description = "A test to check winner percentage is set to 100"
  if ( just_pr ) then print(description) return end

   -- START: Make some test 
  local chk_T
  reset_db()
  local hdrs, outbody, status = mk_test("good_basic1.lua")
  assert(status == 200)
  local test_id_1 = get_test_id(hdrs)
  local T1 = get_test_info(test_id_1)
  assert(test_id_1 == tonumber(T1.id))
  -- for k, v in pairs(T1) do print(k, v) end 

  -- Publish
  S.publish(test_id_1)
  -- Start
  S.start(test_id_1)
  -- Terminate
  local V = T1.Variants
  local vidx = math.random(#V)
  T1.Winner = V[vidx].name
  S.terminate(test_id_1, T1)
  local T1 = get_test_info(test_id_1)
  assert(test_id_1 == tonumber(T1.id))
  -- Extract the percentage of the winner variant
  local T2 = get_test_info(test_id_1)  
  --local winner_variant_data = get_variant(test_id_1, T1.Winner)
    for k, v in pairs(T2.Variants) do
      if ( k == T2.Variants[T1.Winner] ) then
        assert( v == "100") 
       --else
        --assert(v == "0")
      end
     end
  print("Test t1 succeeded")
end



--===================================================
tests.t1() -- TODO: DELETE ONCE I FIGURE OUT WHY TESTRUNNER NOT WORKING HERE
return tests
