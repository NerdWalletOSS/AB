extern int
init(
    void
    );
extern int 
set_seed(
    const char *str,
    uint64_t *ptr_seed
    );
extern size_t 
WriteMemoryCallback(
    void *contents, 
    size_t size, 
    size_t nmemb, 
    void *userp
    );
extern int 
alpha_range(
    uint8_t x
    );
extern int
setup_curl(
    const char *method,
    char *write_buffer,
    const char *server,
    int port,
    const char *url,
    const char *health_url,
    uint32_t timeout_ms,
    CURL **ptr_ch,
    struct curl_slist **ptr_curl_hdrs
    )
  ;
