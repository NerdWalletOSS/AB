#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "get_body.h"

int 
get_body(
    AB_REQ_TYPE req_type,
    struct evhttp_request *req,
    char body[AB_MAX_LEN_BODY+1],
    int n_body
    )
{
  int status = 0;
  struct evbuffer *inbuf = NULL;

  if ( ( req_type == GetVariant ) ||  ( req_type == GetVariants ) ) {
    return status;
  }
  memset(body, '\0', n_body+1);
  inbuf = evhttp_request_get_input_buffer(req);
  // TODO Why do we need while loop?
  if ( evbuffer_get_length(inbuf) > 0 ) {
    int n;
    n = evbuffer_remove(inbuf, body, n_body);
    if ( n == 0 ) { go_BYE(-1); }
    if ( n >= AB_MAX_LEN_BODY ) { 
        sprintf(g_err, "Post body is larger than maximum allowed size");
        go_BYE(-1);
    }
  }
  /* In this code, we do not verify contents of body */
BYE:
  return status;
}
