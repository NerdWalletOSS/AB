#include "txt_to_I8.h"

//START_FUNC_DECL
int
txt_to_I8(
      const char * const X,
      int64_t *ptr_out
      )
//STOP_FUNC_DECL
{
  int status = 0;
  char *endptr = NULL;
  errno = 0;
  int64_t out = 0;
  if ( ( X == NULL ) || ( *X == '\0' ) ) { go_BYE(-1); }
  if ( !is_valid_chars_for_num(X) ) { 
  fprintf(stderr, "Invalid number [%s]\n", X); go_BYE(-1); }
  out = strtoll(X, &endptr, 10);
  if ( ( endptr != NULL ) && ( *endptr != '\0' ) && ( *endptr != '\n' ) ) { go_BYE(-1); }
  if ( ( out < LLONG_MIN ) || ( out > LLONG_MAX ) ) { go_BYE(-1); }
  if ( ((errno == ERANGE) && ((out == LLONG_MAX) || (out == LLONG_MIN)))
      || ((errno != 0) && (out == 0))) {
    fprintf(stderr, "Could not convert [%s] to I8\n", X);
    go_BYE(-1);
  }

  if (endptr == X) {
    fprintf(stderr, "No digits were found\n"); go_BYE(-1);
  }

  *ptr_out = (int64_t)out;
BYE:
  return status ;
}
