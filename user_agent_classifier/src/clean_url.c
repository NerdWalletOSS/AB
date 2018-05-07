#include "ua_incs.h"
#include "clean_url.h"

int
clean_url(
    const char *in_url, 
    char *out_url,
    size_t n_out
    )
{
  int status = 0;
  unsigned int oidx = 0;
  if ( in_url == NULL ) { go_BYE(-1); }
  for ( const char *cptr = in_url; *cptr != '\0'; cptr++ ) {
    char c = tolower(*cptr);
    if ( isalpha(c) ) { 
      if ( oidx > n_out ) { go_BYE(-1); }
      out_url[n_out++] = c;
    }
  }
  memset(out_url, '\0', n_out);
BYE:
  return status;
}
