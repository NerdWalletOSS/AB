#include "ab_incs.h"
#include "auxil.h"
#include "load_device.h"
#include "ab_globals.h"

#define MAXLINE 1024
int
load_device(
    const char *dev_file,
    DEV_REC_TYPE **ptr_devices,
    uint32_t *ptr_n_devices,
    uint32_t *ptr_other_id // for User-Agents that do not match 
    )
{
  int status = 0;
  FILE *dfp = NULL; 
  DEV_REC_TYPE *devices = NULL; int n_devices = 0;
  char line[MAXLINE];

  dfp = fopen(dev_file, "r");
  return_if_fopen_failed(dfp, dev_file, "r");
  status = num_lines(dev_file, &n_devices); cBYE(status);
  if ( n_devices == 0 ) { go_BYE(-1); }
  devices = malloc(n_devices * sizeof(DEV_REC_TYPE));
  return_if_malloc_failed(devices);
  /* read devices */
  int other_id = -1;
  for ( int i = 0; ; i++ ) {
    if ( feof(dfp) ) { break; }
    memset(line, '\0', MAXLINE);
    char *cptr = fgets(line, MAXLINE, dfp); 
    if ( cptr == NULL ) {  break; }
    if ( i >= n_devices ) { go_BYE(-1); }
    int len = strlen(line);
    if ( line[len-1] != '\n' ) { go_BYE(-1); }
    line[len-1] = '\0';
    char *name = strtok(line,",");
    if ( name == NULL ) { go_BYE(-1); }
    char *endptr;
    char *str_idx = strtok(NULL,",");
    if ( str_idx == NULL ) { go_BYE(-1); }
    int device_id = 0;
    device_id = strtoll(str_idx, &endptr, 10);
    if ( device_id <= 0 ) { go_BYE(-1); }
    memset(devices[i].name, '\0', AB_MAX_LEN_DEVICE+1);
    if ( strlen(name)> AB_MAX_LEN_DEVICE ) { go_BYE(-1); }
    strcpy(devices[i].name, name);
    devices[i].id = device_id;
    if ( strcasecmp(name, "other") == 0 ) { 
      if ( other_id >= 0 ) { go_BYE(-1); }
      other_id = device_id;
    }
  }
  if ( other_id < 0 ) { go_BYE(-1); }
  *ptr_devices   = devices;
  *ptr_n_devices = (uint32_t)n_devices;
  *ptr_other_id = (uint32_t)other_id;
BYE:
  fclose_if_non_null(dfp);
  return status;
}
