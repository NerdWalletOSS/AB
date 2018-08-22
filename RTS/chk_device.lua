local assertx    = require 'lua/assertx'
local ffi        = require 'lua/ab_ffi'
local consts     = require 'lua/ab_consts'
local JSON       = require 'lua/JSON'
-- TODO P3 Should do a better job of keeping this in sync with ua_types.h
ffi.cdef([[
typedef struct _lkp_rec_type {
  uint32_t id;
  char *name;
} LKP_REC_TYPE;
]])
local function chk_device(
  justin_cat_lkp, 
  n_justin_cat_lkp
  )
  assert( justin_cat_lkp )
  assert(type(aux) == "table", "global aux table missing")
  assert(type(aux.states) == "table", "aux.states table missing")
  assert(type(aux.device) == "table", "aux.device table missing")
  assert(type(n_justin_cat_lkp ) == "number")
  assert(n_justin_cat_lkp == #(aux.device))
  justin_cat_lkp = ffi.cast("LKP_REC_TYPE *", justin_cat_lkp);
  for i = 0, n_justin_cat_lkp-1 do
    local cname = ffi.string(justin_cat_lkp[i].name)
    local cid =   justin_cat_lkp[i].id
    assert(aux.device[cid] == cname)
  end
  return true
end
return chk_device
