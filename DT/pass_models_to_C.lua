local ffi = require 'ffi'
local function pass_models_to_C(
  cptr,
  l_model_idx, -- indicating Lua model index
  max_len_model
  )
  assert(mdl_map)
  assert( (max_len_model ) and ( max_len_model  > 0 ))

  local model = mdl_map[l_model_idx]
  assert(#model <= max_len_model)
  local cptr = ffi.cast("char *", cptr)
  ffi.copy(cptr, model, #model)
end
return pass_models_to_C
