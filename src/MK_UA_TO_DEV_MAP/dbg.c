#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include "macros.h"
#include "ab_constants.h"

#define MAX_LEN_DIR_NAME 511


int
main()
{
  int status = 0;
  struct stat filestat;
  char *filename = "/opt/ab/ua_to_dev.bin";
  char *X = NULL; size_t nX = 0;
  int fd = open(filename, O_RDONLY);
  if ( fd < 0 ) { go_BYE(-1); }
  status = fstat(fd, &filestat); cBYE(status);
  nX = filestat.st_size;
  X = (void *)mmap(NULL, (size_t) nX, PROT_READ, MAP_SHARED, fd, 0);
  uint64_t *Y = (uint64_t *)X;
  int nY = nX / sizeof(uint64_t);
  uint64_t chk;
  for ( ; ; ) { 
    scanf("%llu", &chk);
    if ( chk == 0 ) { break; }
    bool found = false;
    for ( int i = 0; i < nY; i++ ) { 
      if ( Y[i] == chk ) { 
        fprintf(stderr, "%llu Found at position %d of %d \n", chk, i, nY);
        found = true;
      }
    }
    if ( !found ) { fprintf(stderr, "Not found. Try again \n"); }
  }
BYE:
  return status;
}
