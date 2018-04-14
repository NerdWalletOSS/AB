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
  int sz_buf = 64;
  char buf[sz_buf];
  uint64_t x, hash, hash1, hash2;
  char X[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    'A', 'B', 'C', 'D', 'E', 'F' };

  if ( guid == NULL ) { go_BYE(-1); }
  if ( sz_guid <= 1 )  { go_BYE(-1); }
  memset(buf, '\0', sz_buf);
  x = RDTSC();
  if (( seed != NULL) && ( *seed != '\0' ) ) {
    snprintf(buf, sz_buf,  "%s%" PRIu64, seed, x);
  }
  else {
    snprintf(buf, sz_buf,  "s%" PRIu64, x);
  }
  spooky_hash128(buf, sz_buf, &hash1, &hash2);
  int n_guid = 0;
  for ( int i = 0; i < sz_guid; i++ ) { 
    // TODO P4 improve following code 
    if ( n_guid == 0 ) { hash = hash1; } 
    if ( n_guid == 16 ) { hash = hash2; } 
    uint8_t val = hash & 0xF;
    hash = hash >> 4;
    if ( n_guid > sz_guid ) { break; }
    guid[n_guid++] = X[val];
  }
BYE:
  return status;
}
