#include <evhttp.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>
extern int 
get_hdr_val(
    struct evhttp_request *req,
    const char *key,
    char *val,
    size_t sz
    );
