#include <evhttp.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>

extern int 
get_body(
    AB_REQ_TYPE api,
    struct evhttp_request *req,
    char body[AB_MAX_LEN_BODY+1],
    int n_body
    );
