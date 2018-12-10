local ffi  = require 'ffi'
local JSON = require 'lua/JSON'
lupa_dt = ffi.load('liblupa_dt')
-- make_feature_vector is a global function
-- post_proc_preds is a global function
local function classify(
  g_interp,
  body
  )
  -- print(body)
  -- local x = assert(JSON:decode(body))
  -- for k, v in pairs(x) do print(k, v) end
  assert(make_feature_vector(body, 
  g_interp.dt_feature_vector, g_interp.n_dt_feature_vector))
  -- call C classification
  local status = lupa_dt.lua_eval_mdl(g_interp)
  assert(status)
  local str = post_proc_preds(g_interp.predictions, g_interp.n_mdl)
  assert(str)
  return str
end
return classify 
