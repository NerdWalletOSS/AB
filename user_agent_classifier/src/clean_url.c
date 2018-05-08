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
  memset(out_url, '\0', n_out+1);
  if ( in_url == NULL ) { go_BYE(-1); }
  for ( const char *cptr = in_url; *cptr != '\0'; cptr++ ) {
    char c = tolower(*cptr);
    if ( isspace(c) ) {
      out_url[oidx++] = ' ';
      continue;
    }
    if ( isalpha(c) ) { 
      if ( oidx > n_out ) { go_BYE(-1); }
      out_url[oidx++] = c;
    }
  }
BYE:
  return status;
}

int
idx_in_model(
    const char *in,
    MODEL_REC_TYPE *M,
    size_t nM,
    int *ptr_idx
    )
{
  int status = 0;
  *ptr_idx = -1;
  if ( M == NULL ) { go_BYE(-1); }
  if ( nM == 0 ) { go_BYE(-1); }
  for ( unsigned int i = 0; i < nM; i++ ) { 
    if ( strcmp(in, M[i].word) == 0 ) {
      *ptr_idx = i;
      break;
    }
  }
BYE:
  return status;
}

