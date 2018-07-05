extern int
load_user_agent_classifier(
    bool disable_ua,
    const char *ua_dir,
    uint32_t *ptr_justin_cat_other_id,

    UA_REC_TYPE **ptr_classify_ua_map, 
    size_t *ptr_len_classify_ua_file, 
    uint32_t *ptr_num_classify_ua_map, 

    LKP_REC_TYPE **ptr_justin_cat_lkp, 
    int *ptr_n_justin_cat_lkp, 

    LKP_REC_TYPE **ptr_os_lkp, 
    int *ptr_n_os_lkp, 

    LKP_REC_TYPE **ptr_browser_lkp, 
    int *ptr_n_browser_lkp, 

    LKP_REC_TYPE **ptr_device_type_lkp, 
    int *ptr_n_device_type_lkp
);
