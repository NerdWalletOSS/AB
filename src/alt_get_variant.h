extern int 
alt_get_variant(
    const char *args
    );
extern int
alt_log_decision(
    const char *uuid,
    uint64_t curr_time,
    uint32_t x_tst_id,
    uint32_t ramp_num,
    uint32_t x_var_id,
    const char * const tracer
    );
extern int
make_guid(
    uint64_t seed,
    uint64_t curr_time,
    char temp_guid[2*AB_GUID_LENGTH],
    uint64_t *ptr_rnum // random number 
    );
