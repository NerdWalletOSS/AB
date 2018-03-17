#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "url.h"
#include "classify_ua.h"
#include "get_and_classify_ua.h"

int 
get_and_classify_ua(
    struct evhttp_request *req,
    uint32_t *ptr_device_type_id,
    uint32_t *ptr_os_id,
    uint32_t *ptr_browser_id,
    uint32_t *ptr_justin_cat_id
    )
{
  int status = 0;

  char user_agent[AB_MAX_LEN_USER_AGENT+1];
  memset(user_agent, '\0', AB_MAX_LEN_USER_AGENT+1);
  *ptr_device_type_id = 0;
  *ptr_os_id          = 0;
  *ptr_browser_id     = 0;
  *ptr_justin_cat_id  = 0;

  struct evkeyvalq *headers = NULL;
  struct evkeyval  *header = NULL;
  headers = evhttp_request_get_input_headers (req);
  header = headers->tqh_first;
  for (header = headers->tqh_first; header; 
      header = header->next.tqe_next) {
    if ( strcasecmp(header->key, "User-Agent") == 0 ) { 
      if ( strlen(header->value ) <= AB_MAX_LEN_USER_AGENT ) { 
        strcpy(user_agent, header->value);
      }
      break; 
    }
  }
  if ( *user_agent != '\0' ) { 
    char *decoded_ua = NULL;
    status = url_decode(user_agent, &decoded_ua);
    if ( status < 0 ) { free_if_non_null(decoded_ua); go_BYE(-1); }
    status = classify_ua(decoded_ua, 
        g_classify_ua_map, g_num_classify_ua_map,
        ptr_device_type_id, ptr_os_id,
        ptr_browser_id, ptr_justin_cat_id);
    if ( *ptr_justin_cat_id == 0 ) { g_log_bad_user_agent++; }
    free_if_non_null(decoded_ua); 
    // Don't have this be a catastrophic failure
    if ( status < 0 ) { WHEREAMI; status = 0; }
  }
  else {
    g_log_no_user_agent++; // TODO Log error
  }
BYE:
  return status;
}
