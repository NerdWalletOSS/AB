local ffi  = require 'ffi'
local JSON = require 'lua/JSON'
lupa_dt = ffi.load('liblupa_dt')
-- make_feature_vector is a global function
-- post_proc_preds is a global function
local function classify(
  body
  )
  -- print(body)
  -- local x = assert(JSON:decode(body))
  -- for k, v in pairs(x) do print(k, v) end
  local fvec   = ffi.cast("float *", g_interp[0].dt_feature_vector)
  local n_fvec = g_interp[0].n_dt_feature_vector
  assert(make_feature_vector(body, fvec, n_fvec))
  -- call C classification
  local status = lupa_dt.lua_eval_mdl(g_interp)
  assert(status)
  local str = post_proc_preds(g_interp.predictions, g_interp.n_mdl)
  assert(str)
  return str
end
return classify 
