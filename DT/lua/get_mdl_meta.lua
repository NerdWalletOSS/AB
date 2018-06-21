local assertx          = require "lua/assertx"
local cache            = require "lua/cache"
local ffi              = require "ffi"
local JSON             = require "lua/JSON"

local function get_mdl_meta(
  out_buf, -- output buffer for debugging 
  sz_out_buf -- size of above
  )
  local mdl_meta = assert(cache.get("mdl_meta")),
    "mdl_meta missing from cache.")
  mdl_meta = assert(JSON:encode(out_feature))
  assertx(#mdl_meta <= sz_out_buf, 
    "len of model metadata string is ",
    #mdl_meta,
    ", which is too long for the buffer with length ",
    sz_out_buf)
  ffi.copy(out_buf, mdl_meta)
  return true
end

return get_mdl_meta
