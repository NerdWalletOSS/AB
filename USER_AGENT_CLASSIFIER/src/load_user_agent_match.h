extern int
load_user_agent_match(

    const char *justin_cat_file,
    LKP_REC_TYPE **ptr_justin_cat_lkp, 
    int *ptr_n_justin_cat_lkp, 

    const char *os_file,
    LKP_REC_TYPE **ptr_os_lkp, 
    int *ptr_n_os_lkp, 

    const char *browser_file,
    LKP_REC_TYPE **ptr_browser_lkp, 
    int *ptr_n_browser_lkp, 

    const char *device_type_file,
    LKP_REC_TYPE **ptr_device_type_lkp, 
    int *ptr_n_device_type_lkp,

    const char *ua_to_dev_map_file,
    UA_REC_TYPE **ptr_classify_ua_map,
    size_t *ptr_len_classify_ua_file,
    uint32_t *ptr_num_classify_ua_map
    );
