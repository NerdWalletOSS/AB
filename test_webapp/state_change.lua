require 'lua/str'
local JSON = require 'lua/JSON'
local plfile = require 'pl.file'
local plpath = require 'pl.path'
local curl = require 'lua/curl'
local mk_test = require 'test_webapp/mk_test'
local reset_db = require 'test_webapp/reset_db'
local get_test_id = require 'test_webapp/get_test_id'
local get_test_info = require 'test_webapp/get_test_info'
local get_error_code = require 'test_webapp/get_error_code'
local states = require 'test_webapp/states'
--==========================
local server = os.getenv("WEBAPP_SERVER") or "localhost"
local ssurl =  -- set state URL 
 "http://" .. server .. ":8080/AB/php/endpoints/endpoint_set_state.php"

local S = {}

local function change_state(
  test_id, 
  new_state, 
  optargs
  )
  assert(test_id) 
  assert(type(test_id) == "number")
  assert(new_state) 
  assert(type(new_state) == "string")
  if ( optargs ) then assert(type(optargs) == "table") end 

  local T = assert(get_test_info(test_id))
<<<<<<< HEAD
  if ( new_state == "dormant" ) then 
    assert ( (T.State == "dormant") or (T.State == "draft"))
  elseif ( new_state == "started" ) then 
    assert ( 
             (T.State == "started") or 
             (T.State == "dormant") or 
             ( (T.State == "terminated") and (T.TestType == "XYTest" ) ) 
             )
  elseif ( new_state == "terminated" ) then 
    assert ( (T.State == "terminated") or (T.State == "started"))
  elseif ( new_state == "archived" ) then 
    -- all is well 
  else
    assert(nil, "Invalid new state = " .. new_state)
  end
=======
>>>>>>> dev
  if ( optargs and optargs.Creator ) then 
    T.Updater = optargs.Creator
  else
    T.Updater = T.Creator
  end
  T.NewState = new_state
  if ( new_state == "terminated" ) then 
    if ( optargs ) and ( optargs.Winner ) then 
      T.Winner = optargs.Winner
    else
      local V = T.Variants
      local vidx = math.random(1, #V)
      T.Winner = V[vidx].name
    end
  end
  local hdrs, outbody, status = curl.post(ssurl, nil, JSON:encode(T))
  -- for k, v in pairs(hdrs) do print(k, v) end 
  -- print(outbody)
<<<<<<< HEAD
  assert(status == 200)
=======
  assert(status == 200, "failed to change " .. T.id .. " to " .. new_state)
>>>>>>> dev
  return true
end

local function publish(test_id, optargs)
  assert(change_state(test_id, "dormant", optargs))
  return true
end

local function start(test_id, optargs)
  assert(change_state(test_id, "started", optargs))
  return true
end

local function terminate(test_id, optargs)
  assert(change_state(test_id, "terminated", optargs))
  return true
end

local function archive(test_id, optargs)
  assert(change_state(test_id, "archived", optargs))
  return true
end

S.publish   = publish
S.start     = start
S.terminate = terminate
S.archive   = archive

return S

