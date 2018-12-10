local ffi = require 'ffi'
local function pass_features_to_C(
  cptr,
  l_feature_idx, -- indicating Lua feature index
  max_len_feature
  )
  assert(dt_features)
  assert( (max_len_feature ) and ( max_len_feature  > 0 ))

  local feature = dt_features[l_feature_idx]
  assert(#feature <= max_len_feature)
  local cptr = ffi.cast("char *", cptr)
  ffi.copy(cptr, feature, #feature)
end
return pass_features_to_C
