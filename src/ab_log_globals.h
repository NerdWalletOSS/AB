#ifdef __AB_MAIN_PROGRAM
#define EXTERN extern
#else
#define EXTERN 
#endif
//--- For Internal log 

EXTERN uint64_t g_log_start_time;

EXTERN uint64_t g_log_no_user_agent;
EXTERN uint64_t g_log_bad_user_agent;

EXTERN uint64_t g_log_ss_calls;
EXTERN uint64_t g_log_ss_bad_code;
EXTERN uint64_t g_log_ss_non_ascii;
EXTERN uint64_t g_log_ss_null_data;
EXTERN uint64_t g_log_ss_bad_json;
EXTERN uint64_t g_log_ss_no_session;

EXTERN uint64_t g_log_missing_test_ab;
EXTERN uint64_t g_log_missing_test_xy;

EXTERN uint64_t g_log_no_test_name; // GetVariant
EXTERN uint64_t g_log_no_test_names; // GetVariants

EXTERN uint64_t g_log_dropped_posts; // when occupancy becomes high
EXTERN uint64_t g_log_posts; // num posts attempted (retries not counted)
EXTERN uint64_t g_log_bad_posts; // to Logger => total failure
EXTERN uint64_t g_log_failed_posts; // to Logger => might success on retry

/* allow some retries. If a post makes 2 attempts and fails on both,
then num_bad_posts is incremented by 1 and num_failed_posts by 2. If
it succeeds on second attempt, then num_failed_posts increments by 1
and num_bad_posts stays as is
*/

EXTERN uint64_t g_log_bad_uuid; 
EXTERN uint64_t g_log_bad_test_type;
EXTERN uint64_t g_log_bad_test_name;

EXTERN uint64_t g_log_get_alt_variant_calls; 
EXTERN uint64_t g_log_get_variant_calls; 
EXTERN uint64_t g_log_get_variants_calls; 
EXTERN uint64_t g_log_router_calls;
EXTERN uint64_t g_log_bad_router_calls;


