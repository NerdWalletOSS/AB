#include <time.h>
#include "dt_incs.h"
#include "rand_file_name.h"

static inline uint64_t RDTSC()
{
  return (uint64_t) clock();
  // TODO P2 We should go back to following. 
  // We went to clock because of Raspberry PI
  // unsigned int hi, lo;
  //   __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
  //     return ((uint64_t)hi << 32) | lo;
}
//START_FUNC_DECL
int
rand_file_name(
    char *buf,
    size_t bufsz
    )
//STOP_FUNC_DECL
{
  int status = 0;
  char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
               'A', 'B', 'C', 'D', 'E', 'F' };
  static uint32_t seed = 0;
  static struct drand48_data buffer;
  if (  bufsz < 47 ) { go_BYE(-1); }
  memset(buf, '\0', bufsz);
  if ( seed == 0 ) { 
    seed = RDTSC();
    srand48_r(seed, &buffer);
  }
  char ct[32];
  memset(ct, '\0', 32);
  for ( int i = 0; i < 4; i++ ) { 
    int64_t t;
    lrand48_r(&buffer, &t);
    memcpy(ct+(i*4), &t, 4);
  }
  int bufidx = 0;
  buf[bufidx++] = '_';
  for ( int i = 0; i < 16; i++ ) {  // 16 bytes
    uint8_t c = ct[i];
    uint8_t c1 = c & 15;
    uint8_t c2 = c >> 4;
    buf[bufidx++] = hex[c1];
    buf[bufidx++] = hex[c2];
  }
  buf[bufidx++] = '.';
  buf[bufidx++] = 'b';
  buf[bufidx++] = 'i';
  buf[bufidx++] = 'n';
BYE:
  return status;
}
// gcc -g rand_file_name.c -I../inc/ -I../gen_inc/
#undef STAND_ALONE
#ifdef STAND_ALONE
int
main()
{
  char X[32];
  for ( int i = 0; i < 100; i++ ) { 
    memset(X, '\0', 64);
    rand_file_name(X, 64);
    fprintf(stderr, "X = %s \n", X);
  }
}
#endif

