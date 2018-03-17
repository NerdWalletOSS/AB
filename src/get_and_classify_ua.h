#include <evhttp.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>

extern int 
get_and_classify_ua(
    struct evhttp_request *req,
    uint32_t *ptr_device_type_id,
    uint32_t *ptr_os_id,
    uint32_t *ptr_browser_id,
    uint32_t *ptr_justin_cat_id
    );
