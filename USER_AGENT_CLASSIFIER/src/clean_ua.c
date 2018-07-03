#include <stdio.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <math.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <float.h>
#include "ua_types.h"
#include "macros.h"
#include "auxil.h"
#include "clean_ua.h"

int
clean_ua(
    const char *in_ua, 
    char *out_ua,
    size_t n_out
    )
{
  int status = 0;
  unsigned int oidx = 0;
  memset(out_ua, '\0', n_out+1);
  if ( in_ua == NULL ) { go_BYE(-1); }
  for ( const char *cptr = in_ua; *cptr != '\0'; cptr++ ) {
    char c = tolower(*cptr);
    if ( isspace(c) ) {
      out_ua[oidx++] = ' ';
      continue;
    }
    if ( isalpha(c) ) { 
      if ( oidx > n_out ) { go_BYE(-1); }
      out_ua[oidx++] = c;
    }
    else {
      if ( ( oidx > 0 ) && ( out_ua[oidx-1] == ' ' ) ) {
        // don;t bother
      }
      else {
        out_ua[oidx++] = ' ';
      }
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

