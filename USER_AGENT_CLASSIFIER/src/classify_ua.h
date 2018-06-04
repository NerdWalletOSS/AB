extern int
classify_ua(
    char *user_agent, 
    UA_REC_TYPE *X, // classify_ua_map
    uint32_t nX,  // num_classify_ua_map
    uint32_t *ptr_device_type_id,
    uint32_t *ptr_os_id,
    uint32_t *ptr_browser_id,
    uint32_t *ptr_justin_cat_id
    );
