#include "dt_incs.h"

#include "auxil.h"
#include "extract_api_args.h"
extern char g_valid_chars_in_url[256]; 

int 
extract_api_args(
    const char *uri, 
    char *api, 
    int sz_api,
    char *args, 
    int sz_args
    )
{
  int status = 0;
  char *cptr = (char *)uri; 
  if ( *cptr != '/' ) { WHEREAMI; goto BYE; } 
  cptr++; // jump over forward slash
  memset(api,  '\0', sz_api+1);
  memset(args, '\0', sz_args+1);
  //-------------------------------------
  for ( int i = 0; ( ( *cptr !=  '?' ) && ( *cptr !=  '\0' ) ); i++ ) {
    if ( i >= sz_api ) { go_BYE(-1); }
    if ( !g_valid_chars_in_url[(uint8_t)(*cptr)] ) { go_BYE(-1); }
    api[i] = *cptr++;
  }
  if ( *cptr == '?' ) { cptr++; /* Skip over '?' */ }
  //--------------------------------------
  for ( int i = 0; *cptr !=  '\0'; i++ ) {
    if ( i >= sz_args ) { go_BYE(-1); }
    if ( !g_valid_chars_in_url[(uint8_t)*cptr] ) { 
      go_BYE(-1); }
    args[i] = *cptr++;
  }
BYE:
  return status;
}
