#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "macros.h"
#include "ua_types.h"
#include "spooky_hash.h"
#include "classify_ua.h"

int
classify_ua(
    char *user_agent, 
    UA_REC_TYPE *X, // classify_ua_map
    uint32_t nX,  // num_classify_ua_map
    uint32_t *ptr_device_type_id,
    uint32_t *ptr_os_id,
    uint32_t *ptr_browser_id,
    uint32_t *ptr_justin_cat_id
    )
{
  int status = 0;
  char *alnum_ua = NULL;

  *ptr_device_type_id = 0;
  *ptr_os_id = 0;
  *ptr_browser_id = 0;
  *ptr_justin_cat_id = 0;
  if ( ( user_agent == NULL ) || ( *user_agent == '\0' ) ) { go_BYE(-1); }
  if ( ( X == NULL ) || ( nX == 0 ) )  { go_BYE(-1); }
  //---------------------------------
  // reduce user agent to alphanumeric characters, lower case
  int len = strlen(user_agent+1);
  alnum_ua = malloc(len);
  return_if_malloc_failed(alnum_ua);
  memset(alnum_ua, '\0', len);
  int idx = 0;
  for ( char *cptr = user_agent; *cptr != '\0'; cptr++ ) { 
    if ( isalnum(*cptr) ) {
      alnum_ua[idx++] = tolower(*cptr);
    }
  }
  int len_alnum_ua = idx;
  //---------------------------------

  uint64_t hash1 = 0, hash2 = 0;
  spooky_hash128(alnum_ua, len_alnum_ua, &hash1, &hash2);
  // Note: We must use hash1 because that is what process_user_agent does
  int mid, lb = 0, ub = nX; bool found = false;
  while ( !found ) { 
    mid = (lb + ub) / 2;
    UA_REC_TYPE *x1 = &(X[mid]);
    uint64_t this_hash = ((uint64_t *)x1)[0];
    if ( this_hash == hash1 ) {
      found = true; 
      *ptr_os_id = X[mid].os_id;
      *ptr_browser_id = X[mid].browser_id;
      *ptr_device_type_id = X[mid].device_type_id;
      *ptr_justin_cat_id = X[mid].justin_cat_id;
    }
    else if ( this_hash > hash1 ) { 
      ub = mid;
    }
    else {
      lb = mid;
    }
    if ( mid == (lb+ub)/2 ) { break; }
    mid = (lb+ub)/2;
  }
BYE:
  free_if_non_null(alnum_ua);
  return status;
}
