local function read_all(file)
  local f = assert(io.open(file, "rb"))
  local content = f:read("*all")
  f:close()
  return content
end
--====================================
require 'DT/dt'
local make_feature_vector = require 'make_feature_vector'
local ffi = require 'ffi'
ffi.cdef([[
void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void * malloc(size_t size);
void free(void *ptr);
]])
--====================================
local n_fvec = get_num_features()
print(n_fvec)
fvec_as_json = read_all("./spam/sample_input.json")
fvec = ffi.C.malloc(n_fvec * ffi.sizeof("float"))
assert(make_feature_vector(fvec_as_json, fvec, n_fvec))
fvec = ffi.cast("float *", fvec)
for i = 1, n_fvec do
  print(fvec[i-1])
end
