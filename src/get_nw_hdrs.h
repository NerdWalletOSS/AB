#include <evhttp.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>
extern int 
get_nw_hdrs(
    struct evhttp_request *req,
    char g_x_caller_client_id[AB_MAX_LEN_HDR_VAL],
    char g_x_cookie_id[AB_MAX_LEN_HDR_VAL]
    );
