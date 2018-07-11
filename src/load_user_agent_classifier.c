#include "ab_incs.h"
#include "auxil.h"
#include "load_lkp.h"
#include "load_classify_ua_map.h"
#include "load_user_agent_classifier.h"


int
load_user_agent_classifier(
    bool disable_ua,
    const char *ua_dir,
    uint32_t *ptr_justin_cat_other_id,

    UA_REC_TYPE **ptr_classify_ua_map, 
    size_t *ptr_len_classify_ua_file, 
    uint32_t *ptr_num_classify_ua_map, 

    LKP_REC_TYPE **ptr_justin_cat_lkp, 
    int *ptr_n_justin_cat_lkp, 

    LKP_REC_TYPE **ptr_os_lkp, 
    int *ptr_n_os_lkp, 

    LKP_REC_TYPE **ptr_browser_lkp, 
    int *ptr_n_browser_lkp, 

    LKP_REC_TYPE **ptr_device_type_lkp, 
    int *ptr_n_device_type_lkp
)
{
  int status = 0;
  int buflen = AB_MAX_LEN_FILE_NAME+64;  // TODO P4 improve
  char buf[buflen+1];

  // Free stuff and clean it up
  *ptr_justin_cat_other_id = -1;
  free_lkp("justin", ptr_justin_cat_lkp, ptr_n_justin_cat_lkp);
  free_lkp("os", ptr_os_lkp, ptr_n_os_lkp);
  free_lkp("browser", ptr_browser_lkp, ptr_n_browser_lkp);
  free_lkp("device_type", ptr_device_type_lkp, ptr_n_device_type_lkp);
  if ( ( *ptr_classify_ua_map != NULL ) && 
      ( *ptr_len_classify_ua_file != 0 ) ) {
    munmap(*ptr_classify_ua_map, *ptr_len_classify_ua_file);
  }
  *ptr_classify_ua_map = NULL;
  *ptr_len_classify_ua_file = 0;
  *ptr_num_classify_ua_map = 0;
  //-------------------------------------------

  if ( disable_ua ) { 
    LKP_REC_TYPE *justin_cat_lkp = NULL;
    *ptr_n_justin_cat_lkp = 1;
    *ptr_justin_cat_other_id = 1;
    justin_cat_lkp = malloc(1 * sizeof(LKP_REC_TYPE));
    justin_cat_lkp[0].name = malloc(16);
    strcpy(justin_cat_lkp[0].name, "Other");
    justin_cat_lkp[0].id = 1;
    *ptr_justin_cat_lkp = justin_cat_lkp;
    goto BYE;
  }

  if ( !isdir(ua_dir) ) { go_BYE(-1); }

  // justin cat file 
  memset(buf, '\0', buflen);
  sprintf(buf, "%s/justin_cat.csv", ua_dir);
  if ( !isfile(buf) ) { go_BYE(-1); }
  status = load_lkp("justin", buf, 
      ptr_justin_cat_lkp, ptr_n_justin_cat_lkp);
  cBYE(status);
  for ( int i = 0; i < *ptr_n_justin_cat_lkp; i++ ) { 
    if ( strcasecmp((*ptr_justin_cat_lkp)[i].name, "Other") == 0 ) {
      *ptr_justin_cat_other_id = (*ptr_justin_cat_lkp)[i].id;
    }
  }
  //--------------------------------------------------------
  // os file 
  memset(buf, '\0', buflen);
  sprintf(buf, "%s/os.csv", ua_dir); 
  if ( !isfile(buf) ) { go_BYE(-1); }
  status = load_lkp("os", buf, ptr_os_lkp, ptr_n_os_lkp);
  cBYE(status);
  //--------------------------------------------------------
  // browser file 
  memset(buf, '\0', buflen);
  sprintf(buf, "%s/browser.csv", ua_dir); 
  if ( !isfile(buf) ) { go_BYE(-1); }
  status = load_lkp("browser", buf, 
      ptr_browser_lkp, ptr_n_browser_lkp);
  cBYE(status);
  //--------------------------------------------------------
  // device_type file 
  memset(buf, '\0', buflen);
  sprintf(buf, "%s/device_type.csv", ua_dir); 
  if ( !isfile(buf) ) { go_BYE(-1); }
  status = load_lkp("device_type", buf, 
      ptr_device_type_lkp, ptr_n_device_type_lkp);
  cBYE(status);
  //--------------------------------------------------------
  // ua_to_dev_map_file
  memset(buf, '\0', buflen);
  sprintf(buf, "%s/ua_to_dev_map.bin", ua_dir);
  if ( !isfile(buf) ) { go_BYE(-1); }
  status = load_classify_ua_map(buf, 
      ptr_classify_ua_map, ptr_len_classify_ua_file, 
      ptr_num_classify_ua_map);
  cBYE(status);
BYE:
  return status;
}
