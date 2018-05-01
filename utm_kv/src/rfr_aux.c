#include "rfr_incs.h"
#include "rfr_aux.h"

int
zero_str(
    STR_REC_TYPE *ptr_in
    )
{
  int status = 0;
  if ( ptr_in == NULL ) { go_BYE(-1); }
  ptr_in->X = NULL;
  ptr_in->nX = 0;
BYE:
  return status;
}

int
pr_str(
    STR_REC_TYPE in
    )
{
  int status = 0;
  if ( ( in.X == NULL ) && ( in.nX > 0 ) ) { go_BYE(-1); }
  size_t idx = 0;
  for ( char *cptr = in.X; idx < in.nX; idx++ ) {
    if ( *cptr == '\0' ) { go_BYE(-1); }
    printf("%c", *cptr);
  }
BYE:
  return status;
}
