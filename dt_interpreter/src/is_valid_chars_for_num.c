#include "dt_incs.h"
#include "is_valid_chars_for_num.h"
bool
is_valid_chars_for_num(
      const char * X
      )
{
  if ( ( X == NULL ) || ( *X == '\0' ) ) { WHEREAMI; return false; }
  for ( char *cptr = (char *)X; *cptr != '\0'; cptr++ ) { 
    if ( isdigit(*cptr) || 
        ( *cptr == '-' )  ||
        ( *cptr == '+' )  ||
        ( *cptr == 'e' )  || /* allow for scientific notation*/
        ( *cptr == '.' ) ) {
      continue;
    }
    return false;
  }
  return true;
}
