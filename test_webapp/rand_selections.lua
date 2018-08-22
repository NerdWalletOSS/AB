local channels = require 'test_webapp/channels'
local admins   = require 'test_webapp/admins'
local plfile   = require 'pl.file'
local plpath   = require 'pl.path'
local trim     = require 'lua/trim'
-- TODO P3 Random selections should be improved
--
local salt = 1
local R = {}

local function rand_perc(
  TestType,
  NumVariants
  )
  math.randomseed(os.time())
  local P = {}
  if ( TestType == "XYTest" ) then
    local sum_perc = 0
    local balance = 100
    local perc
    for i = 1, NumVariants-1 do 
      if ( balance == 0 ) then 
        perc = 0
      else
        perc = math.random(0, balance)
        balance = balance - perc
      end
      P[i] = perc
    end
    -- last guy gets whateveris left over
    P[NumVariants] = balance
  elseif ( TestType == "ABTest" ) then
    local x = math.floor(100.0 / NumVariants)
    local sum = 0
    for i = 1, NumVariants do
      P[i] = x
      sum = sum + x
    end
    assert(sum <= 100)
    -- Note that 1 is always Control in this case
    P[1] = P[1] + (100 - sum)
  else
    assert(nil)
  end
  return P
end

local function rand_name()
  math.randomseed(os.time())
  salt = salt + 1
  return tostring(salt + math.random(1, 1000000000))
end

local function rand_dscr()
  salt = salt + 1
  return tostring(math.random(1, 1000000000) + salt)
end

local function rand_type()
  local x = math.random(1, 2)
  if ( x == 1 ) then return "ABTest" end 
  if ( x == 2 ) then return "XYTest" end 
  assert(nil)
end

local function rand_chnl()
  local nC = #channels
  local cidx = math.random(0, nC)
  if ( cidx == 0 ) then 
    return nil
  else
    return channels[cidx]
  end
end
local function rand_dvsp()
  return math.random(0, 1)
end
local function rand_admn()
  local nA = #admins
  local aidx = math.random(1, nA)
  return admins[aidx]
end
local function rand_vrnt(
  TestType, 
  NumVariants
  )
  local cwd = plpath.currentdir() 
  local cd1 = trim(plfile.read(cwd .. "/custom_data_1.json"))
  local cd2 = trim(plfile.read(cwd .. "/custom_data_2.json"))
  math.randomseed(os.time())
  assert(TestType)
  assert(NumVariants)
  local V = {}
  local lp = "http://localhost:8080/AB/test_webapp/landing_page_"
  local P = assert(rand_perc(TestType, NumVariants))
  for i = 1, NumVariants do 
    local v = {}
    if ( ( TestType == "ABTest" ) and ( i == 1 ) ) then 
      v.name = "Control"
    else
      v.name = rand_name() .. tostring(i)
    end
    v.percentage = assert(P[i])
    if ( TestType == "XYTest" ) then 
      v.url = lp .. tostring(i) .. ".php"
    end
    local x = math.random(1, 10)
    if ( x == 1 ) then 
      v.description = trim(tostring(math.random(1, 1000000000) .. salt))
      local y = math.random(1, 2)
      if ( y == 1 ) then 
        v.custom_data = cd1
      else
        v.custom_data = cd2
      end
    end

    V[i] = v
  end
  return V
end

local function rand_test(X)
  if ( not X ) then X = {} end 
  local T = {}
  T.name = X.name or rand_name()
  T.Channel = X.Channel or rand_chnl()
  T.Creator = X.Creator or rand_admn()
  T.description = X.description or rand_dscr()
  T.TestType = X.TestType or rand_type()
  if ( T.TestType == "ABTest" ) then
    T.is_dev_specific = false
  else
    local x = math.random(1, 2)
    if ( x == 1 ) then T.is_dev_specific = true 
    elseif ( x == 2 ) then T.is_dev_specific = false 
    else assert(nil) end
    
  end
  T.NumVariants = X.NumVariants or math.random(2, 8)
  T.Variants = rand_vrnt(T.TestType, T.NumVariants)
  return T
end

R.rand_name = rand_name
R.rand_chnl = rand_chnl
R.rand_dscr = rand_dscr
R.rand_admn = rand_admn
R.rand_test = rand_test
R.rand_perc = rand_perc
R.rand_dvsp = rand_dvsp
return R
