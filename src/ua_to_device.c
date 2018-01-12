#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "ab_constants.h"
#include "macros.h"
#include "ab_types.h"
#include "auxil.h"
#include "url.h"
#include "ab_globals.h"
#include "ua_to_device.h"


//<hdr>
int
ua_to_device(
    const char *args,
    char *X,
    size_t nX
    )
//</hdr>
{
  int status = 0;
  char in_user_agent[AB_MAX_LEN_USER_AGENT+1];
  char *device = NULL; uint32_t device_id; uint32_t nw;
  char *user_agent = NULL;

  memset(in_user_agent, '\0', AB_MAX_LEN_USER_AGENT+1);
  status = extract_name_value(args, "UserAgent=", '&', 
      in_user_agent, AB_MAX_LEN_USER_AGENT);
  if ( in_user_agent[0] == '\0' ) { go_BYE(-1); }
  status = url_decode(in_user_agent, &user_agent); cBYE(status);
  status = validate_url_chars(user_agent); cBYE(status);
  status = get_device_id(user_agent, &device_id, g_ua_to_dev_map, 
      g_n_ua_to_dev_map);
  if ( ( X != NULL ) && ( nX != 0 ) ) {
    if ( device_id == 0 ) { 
      nw = snprintf(X, nX, "{ \"Device\": \"Unknown\", \"DeviceID\" : \"0\" }");
    }
    else {
      for ( uint32_t i = 0; i < g_n_devices; i++ ) { 
        if ( g_devices[i].id == device_id ) { 
          device = g_devices[i].name;
          break;
        }
      }
      nw = snprintf(X, nX, "{ \"Device\": \"%s\", \"DeviceID\" : \"%d\" }",
          device, device_id);
    }
    if ( nw >= nX ) { go_BYE(-1); }
  }
BYE:
  free_if_non_null(user_agent);
  return status;
}

int
get_device_id(
    char *user_agent, 
    uint32_t *ptr_device_id,
    uint64_t *X,
    uint32_t nX
    )
{
  int status = 0;
  int device_id = 0;
  int mid, lb = 0, ub = nX; bool found = false;
  uint64_t mask = 15; mask = ~mask;
  uint64_t hash1 = 0, hash2 = 0;
  if ( ( user_agent == NULL ) || ( *user_agent == '\0' ) ) { go_BYE(-1); }
  if ( ( X == NULL ) || ( nX == 0 ) )  { go_BYE(-1); }
  char alnum_ua[AB_MAX_LEN_USER_AGENT+1];
  //---------------------------------
  // reduce user agent to alphanumeric characters, lower case
  memset(alnum_ua, '\0', AB_MAX_LEN_USER_AGENT+1);
  int idx = 0;
  for ( char *cptr = user_agent; *cptr != '\0'; cptr++ ) { 
    if ( isalnum(*cptr) ) {
      alnum_ua[idx++] = tolower(*cptr);
    }
  }
  int len_alnum_ua = idx;
  //---------------------------------

  spooky_hash128(alnum_ua, len_alnum_ua, &hash1, &hash2);
  uint64_t masked_hash = hash1 & mask;
  while ( !found ) { 
    mid = (lb + ub) /2;
    uint64_t masked_X = X[mid] & mask;
    if ( masked_X == masked_hash ) {
      found = true; 
      device_id = X[mid] & 15;
      if ( device_id == 0 ) { go_BYE(-1); }
    }
    else if ( masked_X > masked_hash ) {
      ub = mid;
    }
    else {
      lb = mid;
    }
    if ( mid == (lb+ub)/2 ) { break; }
    mid = (lb+ub)/2;
  }
  *ptr_device_id = device_id;
BYE:
  return status;
}
