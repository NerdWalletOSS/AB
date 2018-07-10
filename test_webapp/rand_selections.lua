local channels = require 'test_webapp/channels'
local admins   = require 'test_webapp/admins'
-- TODO P3 Random selections should be improved
local R = {}
local function rand_name()
  return tostring(math.random(1, 1000000000))
end
local function rand_dscr()
  return tostring(math.random(1, 1000000000))
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
local function rand_dev_specific()
  return math.random(0, 1)
end
local function rand_admn()
  local nA = #admins
  local aidx = math.random(1, nA)
  return admins[aidx]
end
local function rand_vrnt(test_type)
  local min_num_variants = 2
  local max_num_variants = 4
  local nV = math.random(min_num_variants, max_num_variants)
  local V = {}
  for i = 1, nV do 
    v = {}
    if ( ( test_type == "ABTest" ) and ( i == 1 ) ) then 
      v.name = "Control"
    else
      v.name = tostring(math.random(0, 1000000000))
    end

    v.name = "0

  
end

R.rand_name = rand_name
R.rand_chnl = rand_chnl
R.rand_dscr = rand_dscr
R.rand_admn = rand_admn
R.rand_dev_specific = rand_dev_specific
return R
