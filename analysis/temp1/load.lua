local Q    = require 'Q'
require 'Q/UTILS/lua/strict'

local datadir = "./data/"
local optargs = { is_hdr = true, use_accelerator = true }
optargs = { is_hdr = true }

local M = dofile './meta_eee.lua'
T_eee = Q.load_csv(datadir .. "eee_1609.txt", M, optargs)

local M = dofile './meta_clk.lua'
T_clk = Q.load_csv(datadir .. "clicks_1609.txt", M, optargs)

for k, v in pairs(T_eee) do v:persist() end 
for k, v in pairs(T_clk) do v:persist() end 
Q.save("/home/subramon/local/Q/meta/temp1.meta.lua")
--[[
local mint = Q.min(T_eee.time):eval()
local maxt = Q.max(T_eee.time):eval()
print(maxt - mint)
print(mint, maxt)
--]]
print("Loaded data")
