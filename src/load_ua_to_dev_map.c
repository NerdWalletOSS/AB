#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "ab_constants.h"
#include "macros.h"
#include "ab_types.h"
#include "mmap.h"
#include "auxil.h"
#include "load_ua_to_dev_map.h"
#include "ab_globals.h"
#include <sys/mman.h>

int
load_ua_to_dev_map(
    char *filename,
    uint64_t **ptr_dev_to_ua_map,
    size_t *ptr_n_dev_to_ua_map,
    uint32_t *ptr_num_dev_to_ua_map
    )
{
  int status = 0;
  char *X = NULL; size_t nX = 0;
  status = rs_mmap(filename, &X, &nX, 0); cBYE(status);
  if ( nX == 0 ) { go_BYE(-1); }
  int n_ua = nX / sizeof(uint64_t);
  if ( ( n_ua * sizeof(uint64_t) ) != nX ) { go_BYE(-1); }
  *ptr_dev_to_ua_map = (uint64_t *)X;
  *ptr_n_dev_to_ua_map = nX;
  *ptr_num_dev_to_ua_map = n_ua;
BYE:
  return status;
}
