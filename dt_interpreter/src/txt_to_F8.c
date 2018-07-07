#include "dt_incs.h"
#include "txt_to_F8.h"
#include "is_valid_chars_for_num.h"

//START_FUNC_DECL
int
txt_to_F8(
      const char * const X,
      double *ptr_out
      )
//STOP_FUNC_DECL
{
  int status = 0;
  char *endptr;
  double out;
  if ( ( X == NULL ) || ( *X == '\0' ) ) { go_BYE(-1); }
  if ( !is_valid_chars_for_num(X) ) { go_BYE(-1); }
  out = strtold(X, &endptr);
  if ( ( *endptr != '\0' ) && ( *endptr != '\n' ) ) { go_BYE(-1); }
  if ( endptr == X ) { go_BYE(-1); }
  if ( ( out < - DBL_MAX ) || ( out > DBL_MAX ) ) { go_BYE(-1); }
  *ptr_out = (double)out;
 BYE:
  return status ;
}
