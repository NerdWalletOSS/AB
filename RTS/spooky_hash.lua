local ffi = require 'lua/ab_ffi'
-- load libs 
ffi.cdef([[
extern int get_test_idx(
    const char *name,
    int test_type,
    int *ptr_test_idx
    );
uint64_t spooky_hash64
(
const void *message,
size_t len,
uint64_t seed
);
uint64_t convert_str_to_u64(const char *s);
]])
local spooky = ffi.load('libspooky_hash.so')
return spooky
