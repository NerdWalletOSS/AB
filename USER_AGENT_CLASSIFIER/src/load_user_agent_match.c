#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "macros.h"
#include "ua_types.h"
#include "load_classify_ua_map.h"
#include "load_lkp.h"
#include "load_user_agent_match.h"

int
load_user_agent_match(

    const char *justin_cat_file,
    LKP_REC_TYPE **ptr_justin_cat_lkp, 
    int *ptr_n_justin_cat_lkp, 

    const char *os_file,
    LKP_REC_TYPE **ptr_os_lkp, 
    int *ptr_n_os_lkp, 

    const char *browser_file,
    LKP_REC_TYPE **ptr_browser_lkp, 
    int *ptr_n_browser_lkp, 

    const char *device_type_file,
    LKP_REC_TYPE **ptr_device_type_lkp, 
    int *ptr_n_device_type_lkp,

    const char *ua_to_dev_map_file,
    UA_REC_TYPE **ptr_classify_ua_map,
    size_t *ptr_len_classify_ua_file,
    uint32_t *ptr_num_classify_ua_map
    )
{
  int status = 0;

  // justin cat file 
  if ( *justin_cat_file != '\0' ) { 
    status = load_lkp(justin_cat_file, ptr_justin_cat_lkp, 
        ptr_n_justin_cat_lkp);
    cBYE(status);
  }
  //--------------------------------------------------------
  // os file 
  if ( *os_file != '\0' ) { 
    status = load_lkp(os_file, ptr_os_lkp, ptr_n_os_lkp); cBYE(status);
  }
  //--------------------------------------------------------
  // browser file 
  if ( *browser_file != '\0' ) { 
    status = load_lkp(browser_file, ptr_browser_lkp, ptr_n_browser_lkp);
    cBYE(status);
  }
  //--------------------------------------------------------
  if ( *device_type_file != '\0' ) { 
    status = load_lkp(device_type_file, ptr_device_type_lkp, 
        ptr_n_device_type_lkp);
    cBYE(status);
  }
  //--------------------------------------------------------
  // ua_to_dev_map_file
  if ( *ua_to_dev_map_file != '\0' ) { 
    status = load_classify_ua_map(ua_to_dev_map_file, 
        ptr_classify_ua_map, ptr_len_classify_ua_file, 
        ptr_num_classify_ua_map);
    cBYE(status);
  }

    /* TODO LATER
     g_justin_cat_other_id = -1;
    for ( int i = 0; i < g_n_justin_cat_lkp; i++ ) { 
      if ( strcasecmp(g_justin_cat_lkp[i].name, "Other") == 0 ) {
        g_justin_cat_other_id = g_justin_cat_lkp[i].id;
      }
    }
    */
BYE:
  return status;
}
