#include <evhttp.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>
extern int 
get_date(
    struct evhttp_request *req,
    char *date,
    size_t sz_date
    );
