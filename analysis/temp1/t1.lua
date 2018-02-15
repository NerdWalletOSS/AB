
local mint = Q.min(T_clk.time):eval()
local maxt = Q.min(T_clk.time):eval()
local difft = maxt - mint
local num_bins = Scalar.new(100, "F8")
local delta = difft / num_bins
local lb = Q.seq({start = mint, by = difft, len = num_bins, qtype = "F8" }):eval()
local ub = Q.vsadd(lb, delta):eval()

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


--[[
The following use case will often present itself
local denom = Q.num_in_range(T_clk.time, nil, lb, ub):eval()
local numer = Q.num_in_range(T_clk.time, T_clk.xxx, lb, ub):eval()
-- will want to eval numer and denom a chunk at a time to avoid
-- re-reading T_clk.time

--]]





