require 'lua/str'
local curl         = require 'lua/curl'
local JSON         = require 'lua/JSON'
local get_test_info= require 'test_webapp/get_test_info'
local R  = require 'test_webapp/rand_selections'
local tbp_url = "localhost:8080/AB/php/endpoints/endpoint_test_basic.php"

--[[
--If you have 2 variants A and B, then P should look like
-- P = [ 60, 40 ] 
--]]
local function set_perc(
  tid,
  inP
  )
  local T = assert(get_test_info(tid))
  T.Updater = T.Creator
  local Variants = T.Variants
  local P 
  if ( not inP ) then 
    P = R.rand_perc(T.TestType, #Variants)
  else
    P = inP
  end
  assert(type(P) == "table")
  assert(#P == #Variants)
  for k1, p in pairs(P) do 
    Variants[k1].percentage = p
  end
  local a, b, c = curl.post(tbp_url, nil, JSON:encode(T))
  assert(c == 200)
  return P
end
return set_perc
