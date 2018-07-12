#include "ab_incs.h"
#include "auxil.h"
#include "spooky_hash.h"
#include "make_guid.h"

//<hdr>
int
make_guid(
    char *seed,
    char *guid,
    int sz_guid
    )
//<hdr>
{
  int status = 0;
  int sz_buf = 63;
  char buf[sz_buf+1];
  uint64_t x = 0, hash = 0, hash1 = 0, hash2 = 0;
  int nX = 16;
  char X[nX+1]; strcpy(X, "0123456789ABCDEF");

  if ( guid == NULL ) { go_BYE(-1); }
  if ( sz_guid < 1 )  { go_BYE(-1); }
  memset(buf, '\0',  sz_buf+1);
  memset(guid, '\0', sz_guid+1);
  x = RDTSC();
  if (( seed != NULL) && ( *seed != '\0' ) ) {
    snprintf(buf, sz_buf,  "%s%" PRIu64, seed, x);
  }
  else {
    snprintf(buf, sz_buf,  "%" PRIu64, x);
  }
  spooky_hash128(buf, strlen(buf), &hash1, &hash2);
  for ( int i = 0; i < sz_guid; i++ ) { 
    // TODO P4 improve following code 
    if ( i == 0 ) { hash = hash1; } 
    if ( i == 16 ) { hash = hash2; } 
    //---------------------------
    uint8_t val = (hash & 0xF);
    if ( val >= nX ) { go_BYE(-1); }
    guid[i] = X[val];
    hash = hash >> 4;
  }
BYE:
  return status;
}
