local ffi = require 'ffi'
lupa_dt = ffi.load('liblupa_dt')
local function release()
  print("========== release called =========")
  lupa_dt.free_interp(g_interp)
end
return release 
