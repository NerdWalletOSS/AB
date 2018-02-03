#!/bin/bash
# gen the doth with the #defines replaced
echo '#include "ab_constants.h"' > ffi.h
cat ab_types.h | grep -v "^#" >> ffi.h
echo "local ffi = require 'ffi'"  > ab_ffi.lua
echo "ffi.cdef([[" >>ab_ffi.lua
gcc -E ffi.h | grep -v "^#">>ab_ffi.lua
echo "void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
size_t strlen(const char *str);
typedef struct {
   char *fpos;
   void *base;
   unsigned short handle;
   short flags;
   short unget;
   unsigned long alloc;
   unsigned short buffincrement;
} FILE;
void * malloc(size_t size);
void free(void *ptr);
FILE *fopen(const char *path, const char *mode);
int fclose(FILE *stream);
int fwrite(void *Buffer,int Size,int Count,FILE *ptr);
int fflush(FILE *stream);">>ab_ffi.lua

echo "]])" >>ab_ffi.lua
echo "return ffi" >>ab_ffi.lua
rm ffi.h
