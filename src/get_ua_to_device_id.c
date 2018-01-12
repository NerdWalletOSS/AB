#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ab_constants.h"
#include "macros.h"
#include "ab_types.h"
#include "ab_globals.h"
#include "url.h"

#include "get_ua_to_device_id.h"
#include "ua_to_device.h"
#include "dump_log.h"

int 
get_ua_to_device_id(
    struct evhttp_request *req,
    uint32_t *ptr_device_idx
    )
{
  int status = 0;

  char user_agent[AB_MAX_LEN_USER_AGENT+1];
  memset(user_agent, '\0', AB_MAX_LEN_USER_AGENT+1);
  uint32_t device_id = 0;
  uint32_t device_idx;
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
    status = get_device_id(decoded_ua, &device_id, 
        g_ua_to_dev_map, g_n_ua_to_dev_map);
    if ( device_id == 0 ) {
      log_bad_user_agent(user_agent);  // TODO Log error
    }
    free_if_non_null(decoded_ua); 
    // Don't have this be a catastrophic failure
    if ( status < 0 ) { 
      WHEREAMI; status = 0; device_idx = g_other_idx; 
    }
    else {
      bool found = true;
      for ( uint32_t i = 0; i < g_n_devices; i++ ) { 
        if ( g_devices[i].id == device_id ) {
          found = true; device_idx = i; break;
        }
      }
      if ( !found ) { 
        device_idx = g_other_idx; 
      }
    }
  }
  else {
    g_log_no_user_agent++; // TODO Log error
    device_idx = g_other_idx; 
  }
  if ( device_idx == UINT_MAX ) { 
    device_idx = g_other_idx;  // TODO Log error 
  }
BYE:
  *ptr_device_idx = device_idx;
  return status;
}
