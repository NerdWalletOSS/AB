dofile '_load.lua'
package.loaded['helpers'] = nil;  H = require 'helpers'
mint, maxt = H.get_min_max(T_eee, time)
local difft = maxt - mint
assert(difft > Scalar.new(60*60*24, "F4")) -- at least one day of data 
-- how much longer after EE can click happen in order to be counted
local time_to_impact = Scalar.new(60*60*24, "F4")
-- eliminate clicks that happen too early or too late
T_clk = H.restrict_by_time(T_clk, "time", mint, maxt+time_to_impact)
--============================
-- eliminate clicks that do not correspond to this experiment
local u1 = Q.sort(Q.clone(T_eee.uuid), "asc")
local temp1 = {}
for k,v in pairs(T_clk) do 
  local v2 = Q.where(v, Q.ainb(T_clk.uuid, u1))
  temp1[k] = v2
end
T_clk = temp1
--====================================
local x = Q.ainb(T_eee.uuid, 

