#include "txt_to_I1.h"

//START_FUNC_DECL
int
txt_to_I1(
      const char * const X,
      int8_t *ptr_out
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
  if ( ( out < SCHAR_MIN ) || ( out > SCHAR_MAX ) ) { go_BYE(-1); }
  if ( ((errno == ERANGE) && ((out == SCHAR_MAX) || (out == SCHAR_MIN)))
      || ((errno != 0) && (out == 0))) {
    fprintf(stderr, "Could not convert [%s] to I1\n", X);
    go_BYE(-1);
  }

  if (endptr == X) {
    fprintf(stderr, "No digits were found\n"); go_BYE(-1);
  }

  *ptr_out = (int8_t)out;
BYE:
  return status ;
}
