local plfile = require 'pl.file'
local JSON     = require 'lua/JSON'

local init     = require 'lupa/init'
local release  = require 'lupa/release'
local classify = require 'lupa/classify'
-- make_feature_vector is a global function
local g_interp = init('../DT/spam', "random_forest")
body = plfile.read("../DT/spam/sample_input.json")
assert(JSON:decode(body))
local rslt
for i = 1, 100 do
  rslt = classify(g_interp, body)
  print(i)
end
print("Result = ", rslt)
release(g_interp)
print("Success")
os.exit() -- Needed because of LuaJIT and OpenMP
