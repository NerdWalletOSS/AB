#include <stdio.h>
#include <inttypes.h>
#include "convert_str_to_u64.h"

uint64_t 
convert_str_to_u64(
    const char *s
    ) 
{
  uint64_t x = 0;
  sscanf(s, "%"SCNu64, &x);
  return x;
}
// int main( int argc, char **argv) {
//   char* s = argv[1];
//   printf("Input %s\n", s);
//   uint64_t res =  0;
//   res = convert_to_64(s);
//   printf("BEGIN:%"PRIu64"\n", res);
// }
