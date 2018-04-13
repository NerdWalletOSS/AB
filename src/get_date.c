#include "ab_incs.h"
#include "auxil.h"
#include "get_date.h"

int 
get_date(
    struct evhttp_request *req,
    char *date,
    size_t sz_date
    )
{
  int status = 0;

  if ( date == NULL ) { go_BYE(-1); }
  if ( sz_date <= 1 ) { go_BYE(-1); }
  memset(date, '\0', sz_date+1);
  struct evkeyvalq *headers = NULL;
  struct evkeyval  *header = NULL;
  headers = evhttp_request_get_input_headers (req);
  header = headers->tqh_first;
  for (header = headers->tqh_first; header; 
      header = header->next.tqe_next) {
    if ( strcasecmp(header->key, "Date") == 0 ) { 
      if ( strlen(header->value ) <= sz_date ) { 
        strcpy(date, header->value);
      }
      break; 
    }
  }
BYE:
  return status;
}
