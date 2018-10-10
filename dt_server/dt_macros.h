#ifndef __MACROS_H
#define __MACROS_H
#define WHEREAMI {  \
  fprintf(stderr, "Line %3d of File %s \n", __LINE__, __FILE__);  \
}
/*-------------------------------------------------------*/
#define go_BYE(x) { WHEREAMI; status = x ; goto BYE; }
/*-------------------------------------------------------*/
#define cBYE(x) { if ( (x) < 0 ) { go_BYE((x)) } }
#define fclose_if_non_null(x) { if ( (x) != NULL ) { fclose((x)); (x) = NULL; } } 
#define free_if_non_null(x) { if ( (x) != NULL ) { free((x)); (x) = NULL; } }
#define return_if_fopen_failed(fp, file_name, access_mode) { if ( fp == NULL ) { fprintf(stderr, "Unable to open file %s for %s \n", file_name, access_mode); go_BYE(-1); } }
#define return_if_malloc_failed(x) { if ( x == NULL ) { fprintf(stderr, "Unable to allocate memory\n"); go_BYE(-1); } }

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))
#define sqr(X)  ((X) * (X))

#define rs_munmap(X, nX) { \
  if ( ( X == NULL ) && ( nX != 0 ) ) {  WHEREAMI; return(-1); } \
  if ( ( X != NULL ) && ( nX == 0 ) )  { WHEREAMI; return(-1); } \
  if ( X != NULL ) { munmap(X, nX); X = NULL; nX = 0; } \
}

#define unlink_if_non_null(x) { if ( x != NULL ) { unlink( x ); } }

#define mcr_alloc_null_str(x, sz) { \
  x = (char *)malloc(sz * sizeof(char)); \
  return_if_malloc_failed(x); \
  memset(x, '\0', sz); \
}
#endif
