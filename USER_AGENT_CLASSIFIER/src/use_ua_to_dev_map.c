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
#include "load_user_agent_match.h"
#include "spooky_hash.h"

//-------------------------------------------------------
#define MAX_LEN_NAME AB_MAX_LEN_LKP_NAME
#define MAX_LEN_USER_AGENT 511
#define MAXLINE 2047
//-------------------------------------------------------
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  size_t len_ua_to_dev_map_file = 0;
  UA_REC_TYPE *ua_to_dev_map = NULL; uint32_t n_ua_to_dev_map = 0;
  LKP_REC_TYPE *justin_cat_lkp = NULL; int n_justin_cat_lkp = 0;
  LKP_REC_TYPE *os_lkp = NULL; int n_os_lkp = 0;
  LKP_REC_TYPE *browser_lkp = NULL; int n_browser_lkp = 0;
  LKP_REC_TYPE *device_type_lkp = NULL; int n_device_type_lkp = 0;

  if ( argc != 2 ) { go_BYE(-1); }
  char *ua_to_dev_map_file   = argv[1]; 
  const char *justin_cat_file = "../data/justin_cat.csv";
  const char *os_file         = "../data/os.csv";
  const char *browser_file    = "../data/browser.csv";
  const char *device_type_file= "../data/device_type.csv"; 


  status = load_user_agent_match(
    justin_cat_file, &justin_cat_lkp, &n_justin_cat_lkp, 
    os_file, &os_lkp, &n_os_lkp, 
    browser_file, &browser_lkp, &n_browser_lkp, 
    device_type_file, &device_type_lkp, &n_device_type_lkp,
    ua_to_dev_map_file, 
    &ua_to_dev_map, &len_ua_to_dev_map_file, &n_ua_to_dev_map);
  cBYE(status);

BYE:
  free_lkp(justin_cat_lkp, n_justin_cat_lkp);
  free_lkp(os_lkp, n_os_lkp);
  free_lkp(browser_lkp, n_browser_lkp);
  free_lkp(device_type_lkp, n_device_type_lkp);
  return status;
}
