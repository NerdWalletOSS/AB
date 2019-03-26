local curl              = require 'lua/curl'
local JSON              = require 'lua/JSON'
local rand_name         = require 'test_rts/rand_name'
local rand_set          = require 'test_rts/rand_set'

local function rnum()
  local time_scale = 1000000000
  return math.floor(math.random() * time_scale)
end

local function rand_test (
  optargs
  )
  local U = "localhost:8000/AddTest"
  local H = nil
  if ( optargs ) then
    assert(type(optargs) == "table")
  else
    optargs = {}
  end
  local T = {}
  T.ramp = 1
  T.has_filters = false -- TODO P2
  T.name = optargs.name or rand_name()
  T.State = optargs.State or rand_set("states")
  T.Channel = optargs.Channel or rand_set("channels")
  T.TestType = optargs.TestType or rand_set("test_types")
  T.is_dev_specific = optargs.is_dev_specific or rand_set("is_dev_specific")
  T.seed  = tostring(optargs.seed or rnum())
  T.external_id  = tostring(optargs.external_id or rnum())
  if ( T.TestType == "ABTest" ) then 
    T.BinType = 'c_to_v_ok_v_to_c_ok_v_to_v_not_ok'
  elseif ( T.TestType == "XYTest" ) then 
    T.BinType = 'anonymous'
  else
    assert(nil)
  end
  T.id  = optargs.id or rnum()
  local Variants =  { {
    percentage =  "50",
    url =  "http://www.google.com",
    is_final = "0",
  },
  {
    percentage =  "30",
    url =  "http://www.yahoo.com",
    is_final = "0",
  },
  {
    percentage =  "20",
    url =  "http://www.cnn.com",
    is_final = "0",
  }, }
  for k, v in ipairs(Variants) do
    if ( k == 1 ) and ( T.TestType == "ABTest" ) then 
      v.name = "Control"
    else
      v.name = string.sub(rand_name(), 1, 15)
    end
    v.id = math.random(1000000000)
    v.url = "http://localhost:8080/AB/php/lp" .. k .. ".html"
  end
  T.Variants = Variants
  T.NumVariants = tostring(#Variants)
  if ( T.TestType == "ABTest" ) then
    assert(Variants[1].name:lower() == "control")
  end
  if ( T.State == "terminated" ) then
    -- Note the -1/+1 shennanigans because of Lua 1 indexing
    local winner_idx = math.random(#Variants)
    for k, v in pairs(Variants) do 
      if ( k == winner_idx ) then 
        v.is_final = "1"
      end
    end
  end
  return T
end
return rand_test
