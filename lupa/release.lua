local ffi = require 'ffi'
lupa_dt = ffi.load('liblupa_dt')
local function release()
  lupa_dt.free_interp(g_interp)
end
return release 
