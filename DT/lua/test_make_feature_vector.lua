local ffi = require 'ffi'
local plfile = require 'pl.file'
require 'DT/dt'
hard_code_config()
update_config()

local make_feature_vector = require 'DT/lua/make_feature_vector'
ffi.cdef([[
void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void * malloc(size_t size);
void free(void *ptr);
   ]])

local tests = {}

local X = {}
local Y = {}
Y.json_file = "mk_fvec_body1.json"
Y.n_fvec = 57
X[1] = Y

tests.t1 = function()
  for k, v in ipairs(X) do 
    local json_file = v.json_file 
    local body = assert(plfile.read(json_file))
    local n_fvec = v.n_fvec
    local fvec = ffi.C.malloc(n_fvec * ffi.sizeof("float"))
    local sz_out_buf = 65536
    local out_buf = ffi.C.malloc(sz_out_buf)
    local is_debug = true
    assert( make_feature_vector(body, fvec, n_fvec, out_buf, sz_out_buf, 
    is_debug))
    fvec = ffi.cast("float *", fvec)
    for i = 1, n_fvec do 
      print(fvec[i-1])
    end
  end
end
tests.t1()
return tests
