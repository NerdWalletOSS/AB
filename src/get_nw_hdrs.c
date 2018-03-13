#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "get_nw_hdrs.h"

int 
get_nw_hdrs(
    struct evhttp_request *req,
    char g_x_caller_client_id[AB_MAX_LEN_HDR_VAL],
    char g_x_cookie_id[AB_MAX_LEN_HDR_VAL]
    )
{
  int status = 0;

  if ( req == NULL ) { go_BYE(-1); }
  memset(g_x_caller_client_id, '\0', AB_MAX_LEN_HDR_VAL+1);
  memset(g_x_cookie_id, '\0', AB_MAX_LEN_HDR_VAL+1);
  struct evkeyvalq *headers = NULL;
  struct evkeyval  *header = NULL;
  headers = evhttp_request_get_input_headers (req);
  header = headers->tqh_first;
  bool found1 = false, found2 = false;
  for (header = headers->tqh_first; header; 
      header = header->next.tqe_next) {
    if ( strcasecmp(header->key, "X-Caller-Client-ID") == 0 ) { 
      if ( strlen(header->value ) <= AB_MAX_LEN_HDR_VAL ) { 
        strcpy(g_x_caller_client_id, header->value);
      }
      found1 = true;
    }
    if ( strcasecmp(header->key, "X-Cookie-ID") == 0 ) { 
      if ( strlen(header->value ) <= AB_MAX_LEN_HDR_VAL ) { 
        strcpy(g_x_cookie_id, header->value);
      }
      found2 = true;
    }
    if ( found1 && found2 ) { break; }
  }
BYE:
  return status;
}
