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

extern int 
UI8_srt_compare(
    const void *ii, 
    const void *jj
    );
//-------------------------------------------------------
//<hdr>
int 
UI8_srt_compare(
    const void *ii, 
    const void *jj
    )
//</hdr>
{ 
  uint64_t val1 = *((uint64_t *)ii);
  uint64_t val2 = *((uint64_t *)jj);

  /* Output in ascending order */
  if ( val1 > val2 )  {
    return (1);
  }
  else {
    return (-1);
  }
}

typedef struct _dev_map_rec_type { 
  char justin_cat[32];
  char device[32];
  char device_type[32];
  char os_type[32];
  char browser_type[32];
} DEV_MAP_REC_TYPE;
#define MAXLINE 2047
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  FILE *afp = NULL; FILE *ofp = NULL; FILE *dmfp = NULL;
  DEV_REC_TYPE *devices = NULL; uint32_t n_devices = 0;
  spooky_state g_spooky_state;

  uint64_t g_seed1 = 961748941; // large prime number
  uint64_t g_seed2 = 982451653; // some other large primenumber
  spooky_init(&g_spooky_state, g_seed1, g_seed2);
  char line[MAXLINE+1];
  char *X = NULL; size_t nX = 0;
  char device[AB_MAX_LEN_DEVICE+1];
  DEV_MAP_REC_TYPE *dev_map = NULL; int n_dev_map = 0;
  int bufsz = 32;
  uint32_t other_id = -1;
  char buf[bufsz]; 
  char ua[AB_MAX_LEN_USER_AGENT+1];
  char alnum_ua[AB_MAX_LEN_USER_AGENT+1];

  if ( argc != 5 ) { go_BYE(-1); }
  
  char *dev_file     = argv[1]; 
  char *dev_map_file = argv[2]; 
  char *agent_file   = argv[3]; 
  char *output_file  = argv[4]; 
  dmfp = fopen(dev_map_file, "r");
  return_if_fopen_failed(dmfp, dev_map_file, "r");
  afp = fopen(agent_file, "r");
  return_if_fopen_failed(afp, agent_file, "r");
  ofp = fopen(output_file, "wb");
  return_if_fopen_failed(ofp, output_file, "wb");
  /* read devices */
  status = load_device(dev_file, &devices, &n_devices, &other_id); 
  cBYE(status);
  // read device map 
  status = num_lines(dev_map_file, &n_dev_map); cBYE(status);
  if ( n_dev_map == 0 ) { go_BYE(-1); }
  dev_map = malloc(n_dev_map * sizeof(DEV_REC_TYPE));
  return_if_malloc_failed(dev_map);
  for ( int i = 0; i < n_dev_map; i++ ) { 
    memset(line, '\0', MAXLINE);
    char *cptr = fgets(line, MAXLINE, dmfp); 
    if ( cptr == NULL ) {  break; }
    strtolower(line);
    cptr = line;
    if ( line[strlen(line)-1] == '\n' ) { 
      line[strlen(line)-1] = '\0';
    }
    status = read_to_chars(&cptr, ",\n", buf, bufsz);cBYE(status);
    strcpy(dev_map[i].justin_cat, buf); 
    status = read_to_chars(&cptr, ",\n", buf, bufsz);cBYE(status);
    strcpy(dev_map[i].device, buf); 
    status = read_to_chars(&cptr, ",\n", buf, bufsz);cBYE(status);
    strcpy(dev_map[i].device_type, buf); 
    status = read_to_chars(&cptr, ",\n", buf, bufsz);cBYE(status);
    strcpy(dev_map[i].os_type, buf); 
    status = read_to_chars(&cptr, ",\n", buf, bufsz);cBYE(status);
    strcpy(dev_map[i].browser_type, buf); 
  }
  // device_id must be 1, 2, 3, ...
  for ( unsigned int i = 0; i < (unsigned int)n_devices; i++ ) { 
    bool found = true;
    if ( devices[i].device_id == (i+1) ) { found = true; break; } 
    if ( !found ) { go_BYE(-1); }
  }
  /* read user agent  */
  for ( int i = 0; ; i++ ) {
    char device_type[bufsz];
    char os_type[bufsz];
    char browser_type[bufsz];
    char justin_cat[bufsz];

    memset(device, '\0', AB_MAX_LEN_DEVICE+1);
    memset(device_type, '\0', bufsz);
    memset(os_type, '\0', bufsz);
    memset(browser_type, '\0', bufsz);
    memset(ua, '\0', AB_MAX_LEN_USER_AGENT+1);
    memset(justin_cat, '\0', bufsz);

    if ( feof(afp) ) { break; }
    memset(device,'\0', AB_MAX_LEN_DEVICE+1);
    memset(line, '\0', MAXLINE);
    char *cptr = fgets(line, MAXLINE, afp); 
    if ( cptr == NULL ) {  break; }
    // TODO Put better code when to skip header if ( i == 0 ) { continue; } // skip header
    strtolower(line);
    int len = strlen(line);
    if ( line[len-1] != '\n' ) { 
      go_BYE(-1); 
    }
    line[len-1] = '\0';
    cptr = line;
    status = read_to_chars(&cptr, ",\n", device, AB_MAX_LEN_DEVICE);cBYE(status);
    status = read_to_chars(&cptr, ",\n", device_type, bufsz);cBYE(status);
    status = read_to_chars(&cptr, ",\n", os_type, bufsz);cBYE(status);
    status = read_to_chars(&cptr, ",\n", browser_type, bufsz);cBYE(status);
    status = read_to_chars(&cptr, "\n", ua, AB_MAX_LEN_USER_AGENT);cBYE(status);
    if ( ua[0] == '"' ) {
      int ualen = strlen(ua);
      for ( int k = 0; k < ualen-1; k++ ) { 
        ua[k] = ua[k+1];
      }
      ua[ualen-1] = '\0';
      ua[ualen-2] = '\0';
    }
    /*
    if ( strcasecmp(ua, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36") == 0 ) { 
      printf("hello world\n");
    }
    */

    
    // Get Justin's category 
    for ( int j = 0; j < n_dev_map; j++ ) { 
      if ( strcasecmp(dev_map[j].device, device) != 0 ) { continue; }
      if ( strcasecmp(dev_map[j].device_type, device_type) != 0 ) { continue; }
      if ( strcasecmp(dev_map[j].os_type, os_type) != 0 ) { continue; }
      if ( strcasecmp(dev_map[j].browser_type, browser_type) != 0 ) { continue; }
      // If you reach here, you have a match
      strcpy(justin_cat, dev_map[j].justin_cat);
      break;
    }
    if ( *justin_cat == '\0' ) { 
      go_BYE(-1); }
    // Get device_id
    int device_id = -1;
    for ( uint32_t j = 0; j < n_devices; j++ ) {
      if ( strcasecmp(devices[j].device, justin_cat) == 0 ) { 
        device_id = devices[j].device_id;
      }
    }
    if ( device_id <= 0 ) { 
      go_BYE(-1); 
    }
    // reduce user agent to alphanumeric characters, lower case
    memset(alnum_ua, '\0', AB_MAX_LEN_USER_AGENT+1);
    int idx = 0;
    for ( char *xptr = ua; *xptr != '\0'; xptr++ ) { 
      if ( isalnum(*xptr) ) {
        alnum_ua[idx++] = tolower(*xptr);
      }
    }
    int len_alnum_ua = idx;
    
    // fprintf(stderr, "%s,%s\n", device,ua);
    uint64_t hash1 = 0, hash2 = 0, hash3 = 0;
    spooky_hash128(alnum_ua, len_alnum_ua, &hash1, &hash2);
    uint64_t mask = 15;
    mask = ~mask;
    hash2 = hash1 & mask;
    hash3 = hash2 | device_id;
    fwrite(&hash3, sizeof(uint64_t), 1, ofp);
    char temp[1024];
    strcpy(temp, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
    for ( char *xptr = temp; *xptr != '\0'; xptr++ ) { 
      *xptr = tolower(*xptr);
    }
    if ( strcmp(ua, temp) == 0 ) { 
      fprintf(stderr, "hash1 = %llu\n", hash1);
      fprintf(stderr, "hash2 = %llu\n", hash2);
      fprintf(stderr, "hash3 = %llu\n", hash3);
      fprintf(stderr, "len_alnum_ua = %d\n", len_alnum_ua);
      fprintf(stderr, "alnum_ua = %s\n", alnum_ua);
    }
    /*
    if ( i < 10  ) { 
      fprintf(stderr, "%llu,%s\n", (unsigned long long) hash1, ua);
    }
    */

  }
  fclose_if_non_null(ofp);
  status = rs_mmap(output_file, &X, &nX, 1); cBYE(status);
  int n_ua = nX / sizeof(uint64_t);
  if ( ( n_ua * sizeof(uint64_t) ) != nX ) { go_BYE(-1); }
  qsort(X, n_ua, sizeof(uint64_t), UI8_srt_compare);
  munmap(X, nX);

BYE:
  free_if_non_null(devices);
  fclose_if_non_null(afp);
  fclose_if_non_null(ofp);
  fclose_if_non_null(dmfp);
  return status;
}
