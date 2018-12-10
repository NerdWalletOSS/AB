require 'DT/init'
local function read_all(file)
  local f = assert(io.open(file, "rb"))
  local content = f:read("*all")
  f:close()
  return content
end
--====================================
local pass_models_to_C = require 'pass_models_to_C'
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
assert(mdl_map)
cptr = ffi.C.malloc(1024)
pass_models_to_C(cptr, 1, 63)
local str = ffi.string(cptr)
print("str = ", str, "XXXX")
print("SUCCESS")
