#include <evhttp.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>

extern int 
get_body(
    AB_REQ_TYPE api,
    struct evhttp_request *req,
    char *body, // [n_body+1]
    int n_body,
    int *ptr_sz_body
    );
