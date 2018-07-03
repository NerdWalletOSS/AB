#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <sys/mman.h>
#include "macros.h"
#include "ab_types.h"
#include "auxil.h"
#include "mmap.h"
#include "load_lkp.h"
#include "spooky_hash.h"
#include "load_classify_ua_map.h"
#include "classify_ua.h"
#include "mmap.h"

 UA_REC_TYPE *g_classify_ua_map; // Set by C
 uint32_t g_num_classify_ua_map; // Set by C

 uint32_t g_justin_cat_id; 
 uint32_t g_os_id; 
 uint32_t g_browser_id; 
 uint32_t g_device_type_id; 

 LKP_REC_TYPE *g_justin_cat_lkp; 
 int g_n_justin_cat_lkp; 

 LKP_REC_TYPE *g_os_lkp; 
 int g_n_os_lkp; 

 LKP_REC_TYPE *g_browser_lkp; 
 int g_n_browser_lkp; 

 LKP_REC_TYPE *g_device_type_lkp; 
 int g_n_device_type_lkp; 
 char g_valid_chars_in_url[256];
 LKP_REC_TYPE *g_justin_cat_lkp;
 int g_n_justin_cat_lkp; 
 char g_valid_chars_in_ua[256]; 
 char g_valid_chars_in_ab_args[256]; 
#define MAX_LEN_NAME AB_MAX_LEN_LKP_NAME
#define MAXLINE 2047
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  int g_device_type_id, g_os_id, g_browser_id, g_justin_cat_id;

  LKP_REC_TYPE *g_justin_cat_lkp = NULL; int g_n_justin_cat_lkp = 0;
  LKP_REC_TYPE *g_os_lkp = NULL; int g_n_os_lkp = 0;
  LKP_REC_TYPE *g_browser_lkp = NULL; int g_n_browser_lkp = 0;
  LKP_REC_TYPE *g_device_lkp = NULL; int g_n_device_lkp = 0;
  LKP_REC_TYPE *g_device_type_lkp = NULL; int g_n_device_type_lkp = 0;

  size_t nX = 0; 
  UA_REC_TYPE *classify_ua_map = NULL; 
  uint32_t n_classify_ua_map = 0;

  const char *justin_cat_file = "justin_cat.csv";
  const char *os_file         = "os.csv";
  const char *browser_file    = "browser.csv";
  const char *device_file     = "device.csv"; 
  const char *device_type_file= "device_type.csv"; 

  if ( argc != 3 ) { go_BYE(-1); }

  char *ua_to_dev_map_file = argv[1];
  char *ua                 = argv[2];

  /* read justin categories */
  status = load_lkp(justin_cat_file, &g_justin_cat_lkp, &g_n_justin_cat_lkp);
  cBYE(status);
  status = load_lkp(os_file, &g_os_lkp, &g_n_os_lkp);
  cBYE(status);
  status = load_lkp(browser_file, &g_browser_lkp, &g_n_browser_lkp);
  cBYE(status);
  status = load_lkp(device_file, &g_device_lkp, &g_n_device_lkp);
  cBYE(status);
  status = load_lkp(device_type_file, &g_device_type_lkp, &g_n_device_type_lkp);
  cBYE(status);

  status = load_classify_ua_map(ua_to_dev_map_file, 
      &classify_ua_map, &nX, &n_classify_ua_map); cBYE(status);
  
  status = classify_ua(ua, classify_ua_map, n_classify_ua_map,
    &g_device_type_id, &g_os_id, &g_browser_id, &g_justin_cat_id);
  cBYE(status);
  const char *justin_cat = lkp_id_to_name(g_justin_cat_lkp, 
      g_n_justin_cat_lkp, g_justin_cat_id);
  if ( justin_cat == NULL ) { justin_cat = "Unknown"; }

  const char *os = lkp_id_to_name(g_os_lkp, g_n_os_lkp, g_os_id); 
  if ( os == NULL ) { os = "Unknown"; }

  const char *browser = lkp_id_to_name(g_browser_lkp, g_n_browser_lkp, 
      g_browser_id); 
  if ( browser == NULL ) { browser = "Unknown"; }

  const char *device_type = lkp_id_to_name(
      g_device_type_lkp, g_n_device_type_lkp, g_device_type_id); 
  if ( device_type == NULL ) { device_type = "Unknown"; }

  fprintf(stderr, "device_type,os,browser,justin_cat = %s,%s,%s,%s \n", 
    device_type,os,browser,justin_cat);
   

BYE:
  rs_munmap(classify_ua_map, nX);
  return  status;
}
