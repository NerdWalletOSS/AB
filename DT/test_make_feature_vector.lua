require 'DT/init'
local function read_all(file)
  local f = assert(io.open(file, "rb"))
  local content = f:read("*all")
  f:close()
  return content
end
--====================================
local make_feature_vector = require 'make_feature_vector'
local ffi = require 'ffi'
ffi.cdef([[
void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void * malloc(size_t size);
void free(void *ptr);
]])
--====================================
local model_dir = "./eg_xgboost"
assert(load_model(model_dir))
local n_fvec = assert(get_num_features())
assert(n_fvec > 0)
-- fvec_as_json = read_all("./spam/sample_input.json")
fvec_as_json = read_all("./_x.json")
assert(#fvec_as_json > 0)
fvec = ffi.C.malloc(n_fvec * ffi.sizeof("float"))
assert(make_feature_vector(fvec_as_json, fvec, n_fvec))
fvec = ffi.cast("float *", fvec)
for i = 1, n_fvec do
  print(fvec[i-1])
end
