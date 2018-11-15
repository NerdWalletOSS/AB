require 'DT/dt'
local post_proc_preds = require 'post_proc_preds'
local ffi = require 'ffi'
ffi.cdef([[
void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void * malloc(size_t size);
void free(void *ptr);
]])
--====================================
local n_mdls = get_num_models()
print(n_mdls)
opvec = ffi.C.malloc(n_mdls * ffi.sizeof("float"))
opvec = ffi.cast("float *", opvec)
for i = 1, n_mdls do 
  opvec[i-1] = 1.0/ (1 + i)
end
x = post_proc_preds(opvec, n_mdls)
print(x)
