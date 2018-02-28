local cmem   = require 'libcmem' ; 
local Scalar = require 'libsclr' ; 
local Q = require 'Q';
os.execute("rm -f /home/subramon/local/Q/data/_*bin")
dofile('load.lua')
-- package.loaded['helpers'] = nil;  
H = require 'helpers'
mint, maxt = H.get_min_max(T_eee, "time")
local difft = maxt - mint
assert(difft > Scalar.new(60*60*24, "F4")) -- at least one day of data 
-- how much longer after EE can click happen in order to be counted
local time_to_impact = Scalar.new(60*60*24, "F4")
--============================
-- eliminate clicks that do not correspond to this experiment
local u1 = Q.sort(Q.clone(T_eee.uuid), "asc")
print("num(u1) = ", u1:length())
print("num(T_clk) = ", T_clk.uuid:length())
local temp1 = {}
local idx = 1
local len = 0
for k,v in pairs(T_clk) do 
  local v2 = Q.where(v, Q.ainb(T_clk.uuid, u1)):eval()
  print("completed first where")
  temp1[k] = v2
  if ( idx == 1 ) then 
    chk_len = v:length()
  else
    assert(chk_len == v:length())
  end
end
T_clk = temp1
print("all done")
os.exit()
--====================================
-- eliminate clicks that happen too early or too late
T_clk = H.restrict_by_time(T_clk, "time", mint, maxt+time_to_impact)

