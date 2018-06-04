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
#include "load_lkp.h"
#include "load_justin_map.h"
#include "spooky_hash.h"

//-------------------------------------------------------
#define MAX_LEN_NAME AB_MAX_LEN_LKP_NAME
#define MAX_LEN_USER_AGENT 511
#define MAXLINE 2047
//-------------------------------------------------------

//<hdr>
static int 
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

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  FILE *afp = NULL; FILE *ofp = NULL; 
  spooky_state g_spooky_state;

  uint64_t g_seed1 = AB_SEED_1;
  uint64_t g_seed2 = AB_SEED_2;
  spooky_init(&g_spooky_state, g_seed1, g_seed2);
  char *X = NULL; size_t nX = 0;

  JUSTIN_MAP_REC_TYPE *justin_map = NULL; int n_justin_map = 0;
  LKP_REC_TYPE *justin_cat_lkp = NULL; int n_justin_cat_lkp = 0;
  LKP_REC_TYPE *os_lkp = NULL; int n_os_lkp = 0;
  LKP_REC_TYPE *browser_lkp = NULL; int n_browser_lkp = 0;
  LKP_REC_TYPE *device_lkp = NULL; int n_device_lkp = 0;
  LKP_REC_TYPE *device_type_lkp = NULL; int n_device_type_lkp = 0;

  if ( argc != 3 ) { go_BYE(-1); }
  
  char *agent_file   = argv[1]; 
  char *output_file  = argv[2]; 

  const char *justin_map_file = "../data/justin_map.csv";

  const char *justin_cat_file = "../data/justin_cat.csv";
  const char *os_file         = "../data/os.csv";
  const char *browser_file    = "../data/browser.csv";
  const char *device_file     = "../data/device.csv"; 
  const char *device_type_file= "../data/device_type.csv"; 

  afp = fopen(agent_file, "r");
  return_if_fopen_failed(afp, agent_file, "r");
  ofp = fopen(output_file, "wb");
  return_if_fopen_failed(ofp, output_file, "wb");
  /* read justin categories */
  status = load_lkp(justin_cat_file, &justin_cat_lkp, &n_justin_cat_lkp);
  cBYE(status);
  status = load_lkp(os_file, &os_lkp, &n_os_lkp);
  cBYE(status);
  status = load_lkp(browser_file, &browser_lkp, &n_browser_lkp);
  cBYE(status);
  status = load_lkp(device_file, &device_lkp, &n_device_lkp);
  cBYE(status);
  status = load_lkp(device_type_file, &device_type_lkp, &n_device_type_lkp);
  cBYE(status);
  // read mapping between other lkps and justin category
  status = load_justin_map(justin_map_file, &justin_map, &n_justin_map);
  cBYE(status);
  /* read user agent  */
  uint32_t n_bad_user_agent = 0, n_good_user_agent = 0;
  for ( int i = 0; ; i++ ) {
    int nw;
    char line[MAXLINE+1];
    char ua[MAX_LEN_USER_AGENT+1];
    char alnum_ua[MAX_LEN_USER_AGENT+1];
    char device[MAX_LEN_NAME+1];
    char device_type[MAX_LEN_NAME+1];
    char os[MAX_LEN_NAME+1];
    char browser[MAX_LEN_NAME+1];
    char justin_cat[MAX_LEN_NAME+1];

    memset(line, '\0', MAXLINE+1);
    memset(ua, '\0', MAX_LEN_USER_AGENT+1);
    memset(alnum_ua, '\0', MAX_LEN_USER_AGENT+1);
    memset(device, '\0', MAX_LEN_NAME+1);
    memset(device_type, '\0', MAX_LEN_NAME+1);
    memset(os, '\0', MAX_LEN_NAME+1);
    memset(browser, '\0', MAX_LEN_NAME+1);
    memset(justin_cat, '\0', MAX_LEN_NAME+1);


    if ( feof(afp) ) { break; }
    char *cptr = fgets(line, MAXLINE, afp); 
    if ( cptr == NULL ) {  break; }
    // TODO Put better code when to skip header if ( i == 0 ) { continue; } // skip header
    strtolower(line);
    int len = strlen(line);
    if ( line[len-1] != '\n' ) { 
      go_BYE(-1); 
    }
    line[len-1] = '\0';
    cptr = line; int bufsz = MAX_LEN_NAME;
    status = read_to_chars(&cptr, ",\n", device, bufsz);cBYE(status);
    status = read_to_chars(&cptr, ",\n", device_type, bufsz);cBYE(status);
    status = read_to_chars(&cptr, ",\n", os, bufsz);cBYE(status);
    status = read_to_chars(&cptr, ",\n", browser, bufsz);cBYE(status);
    status = read_to_chars(&cptr, "\n", ua, MAX_LEN_USER_AGENT);
    if ( status < 0 ) { 
      printf("bad user agent on Line %d\n", i+1); 
      n_bad_user_agent++;
      continue;
    }
    cBYE(status);
    status = strip_dquotes(ua); cBYE(status);
    
    // Get Justin's category 
    for ( int j = 0; j < n_justin_map; j++ ) { 
      if ( strcasecmp(justin_map[j].device, device) != 0 ) { continue; }
      if ( strcasecmp(justin_map[j].device_type, device_type) != 0 ) { continue; }
      if ( strcasecmp(justin_map[j].os, os) != 0 ) { continue; }
      if ( strcasecmp(justin_map[j].browser, browser) != 0 ) { continue; }
      // If you reach here, you have a match
      strcpy(justin_cat, justin_map[j].justin_cat);
      break;
    }
    if ( *justin_cat == '\0' ) { 
      fprintf(stderr, "ERR: Update justin_map.csv\n"); go_BYE(-1); 
    }
    uint8_t justin_cat_id;
    status = lkp_name_to_id(justin_cat_lkp, n_justin_cat_lkp, 
        justin_cat, &justin_cat_id); cBYE(status);
    uint8_t os_id;
    status = lkp_name_to_id(os_lkp, n_os_lkp, os, &os_id); cBYE(status);
    uint8_t browser_id;
    status = lkp_name_to_id(browser_lkp, n_browser_lkp, 
        browser, &browser_id); cBYE(status);
    uint8_t device_type_id;
    status = lkp_name_to_id(device_type_lkp, n_device_type_lkp, 
        device_type, &device_type_id); cBYE(status);
    // reduce user agent to alphanumeric characters, lower case
    int idx = 0;
    for ( char *xptr = ua; *xptr != '\0'; xptr++ ) { 
      if ( isalnum(*xptr) ) {
        alnum_ua[idx++] = tolower(*xptr);
      }
    }
    int len_alnum_ua = idx;
    uint64_t hash1 = 0, hash2 = 0;
    spooky_hash128(alnum_ua, len_alnum_ua, &hash1, &hash2);
    nw = fwrite(&hash1, sizeof(uint64_t), 1, ofp);
    if ( nw != 1 ) { go_BYE(-1); }
    nw = fwrite(&device_type_id, sizeof(uint8_t), 1, ofp);
    if ( nw != 1 ) { go_BYE(-1); }
    nw = fwrite(&os_id, sizeof(uint8_t), 1, ofp);
    if ( nw != 1 ) { go_BYE(-1); }
    nw = fwrite(&browser_id, sizeof(uint8_t), 1, ofp);
    if ( nw != 1 ) { go_BYE(-1); }
    nw = fwrite(&justin_cat_id, sizeof(uint8_t), 1, ofp);
    if ( nw != 1 ) { go_BYE(-1); }
    n_good_user_agent++;
  }
  fclose_if_non_null(ofp);
  printf("number of bad  user agents = %d \n", n_bad_user_agent);
  printf("number of good user agents = %d \n", n_good_user_agent);
  status = rs_mmap(output_file, &X, &nX, 1); cBYE(status);
  uint32_t rec_size = (4*sizeof(uint8_t)) + sizeof(uint64_t);
  uint32_t n_ua = nX / rec_size;
  if ( n_ua != n_good_user_agent ) { go_BYE(-1); }
  if ( ( n_ua * rec_size ) != nX ) { go_BYE(-1); }
  // Note trick below. rec_sizeis 16 but we use UI8_srt_compare
  // and compare only first 8 bytes
  qsort(X, n_ua, rec_size, UI8_srt_compare);
  munmap(X, nX);

BYE:
  fclose_if_non_null(afp);
  fclose_if_non_null(ofp);
  free_if_non_null(justin_map);
  free_lkp(justin_cat_lkp, n_justin_cat_lkp);
  free_lkp(os_lkp, n_os_lkp);
  free_lkp(browser_lkp, n_browser_lkp);
  free_lkp(device_lkp, n_device_lkp);
  free_lkp(device_type_lkp, n_device_type_lkp);
  return status;
}
