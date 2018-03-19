/*
======
Function 1: _url_strip
Strips the URL of certain trash characters

Input: url (string, char*)

Output: url (string, char*)

Processes:
0. special_chars := '/.-()\'\" +[*?&'
1. url := url after stripping any number of chars in special_chars from the front and rear.
XX not needed 2. tok := strtok(url, delim='/')
3. trash := starting from end, work backwards until you hit a slash
4a. If trash (which is a string) is all digits and has a length of 13 (i.e. a 13-digit number):
        - remove the last 13 chars from url, i.e. remove trash
        - strip special_chars from the front and rear. (i.e same step as 1)
        - return that final product
4b. Else:
        - return url

======

*/
#include "rfr_incs.h"
#include "rfr_aux.h"
#include "strip_url.h"

int
strip_url(
    STR_REC_TYPE in,
    STR_REC_TYPE *ptr_out
    )
{
  int status = 0;
  if ( ptr_out == NULL ) { go_BYE(-1); }
  ptr_out->X  = NULL;
  ptr_out->nX = 0;

  zero_str(ptr_out);
  size_t lidx = 0, ridx = in.nX-1;
  char is_bad_char[256];
  memset(is_bad_char, '\0', 256);

  const char *bad_chars = "/.-()\'\" +[*?&";
  for ( const char *cptr = bad_chars; *cptr != '\0'; cptr++ ) { 
    is_bad_char[(uint8_t)*cptr] = true;
  }

  for ( size_t i = 0; i < in.nX; i++, lidx++ ) { 
    char inchar = in.X[i];
    if ( !is_bad_char[(uint8_t)inchar] ) {
      break;
    }
  }
  for ( int i = in.nX-1; i >= 0; i--, ridx-- ) { 
    char inchar = in.X[i];
    if ( !is_bad_char[(uint8_t)inchar] ) {
      break;
    }
  }
  if ( lidx > ridx ) { return status; }
  ptr_out->X = in.X; ptr_out->X +=  lidx;
  ptr_out->nX = ridx - lidx + 1;
BYE:
  return status;
}
