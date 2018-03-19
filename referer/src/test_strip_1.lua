

require 'ffi'

ffi.cdef([[
typedef struct _str_rec_type {
  char *X;
  size_t nX;
} STR_REC_TYPE;
]])
ffi.cdef([[
extern int
strip_url(
    STR_REC_TYPE in,
    STR_REC_TYPE *ptr_out
    );
]]
)
libh = ffi.load("./referer.so")

input = ffi.cast("STR_REC_TYPE *", ffi.C.malloc(ffi.sizeof("STR_REC_TYPE")))
input[0].X = ....
input[0].nX = 10

output = ffi.cast("STR_REC_TYPE *", ffi.C.malloc(ffi.sizeof("STR_REC_TYPE")))

output[0].X = nil
output[0].nX = 0

libh.strip_url(input[0], output);

assert(....)
