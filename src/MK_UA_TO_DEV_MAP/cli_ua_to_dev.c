#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "ab_constants.h"
#include "ab_types.h"
#include "macros.h"
#include "auxil.h"
#include "mmap.h"
#include "load_device.h"
#include "spooky_hash.h"
#include "load_ua_to_dev_map.h"
#include "ua_to_device.h"

#define MAXLINE 2047
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char alnum_ua[AB_MAX_LEN_USER_AGENT+1];

  if ( argc != 4 ) { go_BYE(-1); }
  char *dev_file           = argv[1];
  char *ua_to_dev_map_file = argv[2];
  char *ua                 = argv[3];
  uint64_t *ua_to_dev_map;
  uint32_t n_ua_to_dev_map;

  uint32_t device_id, other_id;
  DEV_REC_TYPE *devices;
  uint32_t n_devices;
  
  status = load_device(dev_file, &devices, &n_devices, &other_id); 
  cBYE(status);
  status = load_ua_to_dev_map(ua_to_dev_map_file, 
      &ua_to_dev_map, &n_ua_to_dev_map);
    cBYE(status);
  status = get_device_id(ua, &device_id, ua_to_dev_map, n_ua_to_dev_map);
  cBYE(status);
  fprintf(stderr, "device_id = %d \n", device_id);

BYE:
  return  status;
}
