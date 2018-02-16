local cmem   = require 'libcmem' ; 
local Scalar = require 'libsclr' ; 
local helpers = {}
local function get_min_max(T, f)
  local mint = Q.min(T[f]):eval()
  local maxt = Q.max(T[f]):eval()
  return mint, maxt
end
helpers.find_test_period = find_test_period
--============================================
local function restrict_by_time(T, timefld, mint, maxt)
  local geq_mint = Q.vsgeq(T[timefld], mint)
  local leq_maxt = Q.vsleq(T[timefld], maxt)
  local is_good = Q.vvand(geq_mint, leq_maxt)
  local x, y = Q.sum(is_good):eval()
  if ( x == Scalar.new(0, "I") ) then return nil end 
  local Tnew = {}
  for k, v in pairs(T) do 
    Tnew[k] = Q.where(T[k], is_good)
  end
  return Tnew
end
helpers.restrict_by_time = restrict_by_time
--============================================
local function make_ranges(mint, difft, num_bins)
  local delta = difft / num_bins
  local subsample = 4 -- 4 is somewhat arbitrary
  local by = delta / subsample
  local len = num_bins * subsample
  local lb = Q.seq({start = mint, by = by, len = len, qtype = "F8" })
  local ub = Q.vsadd(lb, by)
  return lb, ub
end
helpers.make_ranges = make_ranges
--============================================

local function xxx()
  local variants = {}
  variants[1] = 1234 -- TODO undo hard coding
  variants[2] = 1235 -- TODO undo hard coding
  for k, v in ipairs(variants) do 
    local vid_a = Scalar.new(v, "I4")
    local is_a  = Q.conv(Q.vseq(T_eee.variant_id, vid_a), "I1")
    local n_a = Q.num_in_range(T_eee.time, is_a, lb, ub):eval()
    -- variants[k].n = n_a
  end
  
  local x = Q.num_in_range(T_eee.time, nil, lb, ub):eval()
end
helpers.xxx = xxx
--============================================
return helpers



--[[
The following use case will often present itself
local denom = Q.num_in_range(T_clk.time, nil, lb, ub):eval()
local numer = Q.num_in_range(T_clk.time, T_clk.xxx, lb, ub):eval()
-- will want to eval numer and denom a chunk at a time to avoid
-- re-reading T_clk.time

--]]





