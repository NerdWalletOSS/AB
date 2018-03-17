extern int 
lkp_name_to_id(
    LKP_REC_TYPE *lkp,
    int n_lkp,
    char *name,
    uint8_t *ptr_lkp_id
    );
extern int
load_lkp(
    const char *lkp_file,
    LKP_REC_TYPE **ptr_lkp,
    int *ptr_n_lkp
    );
extern char *
lkp_id_to_name(
    LKP_REC_TYPE *lkp,
    int n_lkp,
    uint32_t id
    );

