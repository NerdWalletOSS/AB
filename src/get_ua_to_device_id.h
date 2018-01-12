#include <evhttp.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>

extern int 
get_ua_to_device_id(
    struct evhttp_request *req,
    uint32_t *ptr_device_idx
    );
