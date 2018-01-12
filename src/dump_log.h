extern int
dump_log(
    const char * const args
    );
extern char *
log_common(
    uint64_t log_type,
    int *ptr_rec_size
    );
extern void
log_bad_user_agent(
    const char * const user_agent
    );
extern void
log_missing_test(
    const char * const test_name,
    uint64_t test_type,
    uint64_t external_id
    );
extern void
log_ss_calls(
    const char * const uuid,
    uint64_t response_time,
    uint64_t http_code,
    uint64_t is_null_data,
    uint64_t is_bad_data,
    const char *const ss_response
    );
