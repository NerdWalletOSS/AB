local Q = require 'Q'

require 'Q/UTILS/lua/strict'

local datadir = "./data/"
local optargs = { is_hdr = true, use_accelerator = true }
optargs = { is_hdr = true }

local M = dofile './meta_eee.lua'
local T_eee = Q.load_csv(datadir .. "eee_1609.txt", M, optargs)

local M = dofile './meta_clk.lua'
local T_clk = Q.load_csv(datadir .. "clicks_1609.txt", M, optargs)

print("ALL DONE")
