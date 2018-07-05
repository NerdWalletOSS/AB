#include "ab_incs.h"
#include "auxil.h"
#include "load_lkp.h"
#include "load_classify_ua_map.h"
#include "load_user_agent_classifier.h"

int
load_user_agent_classifier(
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
  char buf[AB_MAX_LEN_FILE_NAME+1+64]; // TODO P4 improve

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

  if ( !isdir(ua_dir) ) { go_BYE(-1); }

  // justin cat file 
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
  sprintf(buf, "%s/os.csv", ua_dir); 
  if ( !isfile(buf) ) { go_BYE(-1); }
  status = load_lkp("os", buf, ptr_os_lkp, ptr_n_os_lkp);
  cBYE(status);
  //--------------------------------------------------------
  // browser file 
  sprintf(buf, "%s/browser.csv", ua_dir); 
  if ( !isfile(buf) ) { go_BYE(-1); }
  status = load_lkp("browser", buf, 
      ptr_browser_lkp, ptr_n_browser_lkp);
  cBYE(status);
  //--------------------------------------------------------
  // device_type file 
  sprintf(buf, "%s/device_type.csv", ua_dir); 
  if ( !isfile(buf) ) { go_BYE(-1); }
  status = load_lkp("device_type", buf, 
      ptr_device_type_lkp, ptr_n_device_type_lkp);
  cBYE(status);
  //--------------------------------------------------------
  // ua_to_dev_map_file
  sprintf(buf, "%s/ua_to_dev_map.bin", ua_dir);
  if ( !isfile(buf) ) { go_BYE(-1); }
  status = load_classify_ua_map(buf, 
      ptr_classify_ua_map, ptr_len_classify_ua_file, 
      ptr_num_classify_ua_map);
  cBYE(status);
BYE:
  return status;
}
