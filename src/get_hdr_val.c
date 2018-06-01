#include "ab_incs.h"
#include "auxil.h"
#include "get_hdr_val.h"

int 
get_hdr_val(
    struct evhttp_request *req,
    const char *key,
    char *val,
    size_t sz
    )
{
  int status = 0;

  if ( val == NULL ) { go_BYE(-1); }
  if ( sz <= 1 ) { go_BYE(-1); }
  memset(val, '\0', sz+1);
  struct evkeyvalq *headers = NULL;
  struct evkeyval  *header = NULL;
  headers = evhttp_request_get_input_headers (req);
  header = headers->tqh_first;
  for (header = headers->tqh_first; header; 
      header = header->next.tqe_next) {
    if ( strcasecmp(header->key, key) == 0 ) { 
      if ( strlen(header->value ) <= sz ) { 
        strcpy(val, header->value);
      }
      break; 
    }
  }
BYE:
  return status;
}
