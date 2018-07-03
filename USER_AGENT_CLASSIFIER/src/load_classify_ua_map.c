#include "ab_incs.h"
#include "mmap.h"
#include "auxil.h"
#include "load_classify_ua_map.h"

int
load_classify_ua_map(
    const char *filename,
    UA_REC_TYPE **ptr_classify_ua_map,
    size_t *ptr_len_file,
    uint32_t *ptr_num_classify_ua_map
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  status = rs_mmap(filename, &X, &nX, 0); cBYE(status);
  if ( nX == 0 ) { go_BYE(-1); }
  uint32_t n_ua = nX / sizeof(UA_REC_TYPE);
  if ( ( n_ua * sizeof(UA_REC_TYPE) ) != nX ) { go_BYE(-1); }
  *ptr_classify_ua_map = (UA_REC_TYPE *)X;
  *ptr_len_file = nX;
  *ptr_num_classify_ua_map = n_ua;
BYE:
  return status;
}
