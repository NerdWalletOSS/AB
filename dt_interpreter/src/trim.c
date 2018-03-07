#include "dt_incs.h"
#include "trim.h"
extern int 
trim(
    char * restrict inbuf,  /* input */
    char * restrict outbuf, 
    int n /* number of bytes allocated */
    )
{
  int status = 0;
  if ( inbuf == NULL ) { go_BYE(-1); }
  if ( outbuf == NULL ) { go_BYE(-1); }
  if ( n <= 1 ) { go_BYE(-1); }
  int start_idx, stop_idx;
  // START: trim lbuf into buf
  if ( inbuf[n-1] != '\0' ) { go_BYE(-1); }
  memset(outbuf, '\0', n);
  for ( start_idx = 0; start_idx < n; start_idx++ ) { 
    if ( !isspace(inbuf[start_idx]) ) { break; }
  }
  // TODO Do not start at max and work downwards
  // Start from where you left off and keep going 
  for ( stop_idx = n-1; stop_idx >= 0; stop_idx-- ) { 
    if ( !( ( inbuf[stop_idx] == '\0' ) || ( isspace(inbuf[stop_idx]) ) ) ) {
      break;
    }
  }
  int k = 0;
  for ( int j = start_idx; j <= stop_idx; j++, k++ ) { /* note the <= */
    outbuf[k] = inbuf[j];
  }
BYE:
  return status;
}
