#ifdef __AB_MAIN_PROGRAM
#define EXTERN extern
#else
#define EXTERN 
#endif
//--- For Internal log. All set by C

EXTERN uint64_t g_log_start_time;
/*C time (micro-seconds since epoch) at which server was started */

EXTERN uint64_t g_log_no_user_agent;
/*C number of times no user agent was extracted */
EXTERN uint64_t g_log_bad_user_agent;
/*C number of times user agent was not matched to known device */
EXTERN uint64_t g_log_bad_ab_args;
/*C number of times bad arguments to GetVariant(s) */

EXTERN uint64_t g_log_ss_calls;
/*C number of calls to session service */
EXTERN uint64_t g_log_ss_bad_code;
/*C number of times session service returned bad HTTP code */
EXTERN uint64_t g_log_ss_timeout;
/*C number of times session service timed out */
EXTERN uint64_t g_log_ss_non_ascii;
/*C number of times session service response had non ascii characters */
EXTERN uint64_t g_log_ss_null_data;
/*C number of times session service response was null */
EXTERN uint64_t g_log_ss_bad_json;
/*C number of times session service response was invalid JSON */
EXTERN uint64_t g_log_ss_no_session;
/*C number of times session service did not have info for UUID */
EXTERN uint64_t g_log_ss_bad_calls;
/*C number of times \verb+l_chk_exclude()+ failed */

EXTERN uint64_t g_log_missing_test;

EXTERN uint64_t g_log_no_test_name; 
/*C TODO */
EXTERN uint64_t g_log_no_test_names; 
/*C TODO */
EXTERN uint64_t g_log_no_uuid; 
/*C number of times no UUID in GetVariant(s) call */
EXTERN uint64_t g_log_no_test_type; 
/*C number of times no TestType in GetVariant(s) call */
EXTERN uint64_t g_log_no_tracer; 
/*C number of times no Tracer in GetVariant(s) call */

EXTERN uint64_t g_log_dropped_posts; 
/*C number of logger POSTS dropped because occupancy in queue is high */
EXTERN uint64_t g_log_posts; 
/*C number of logger POSTS attempted (retries not counted) */
EXTERN uint64_t g_log_bad_posts; 
/*C number of bad logger POSTS i.e.,  total failure */
EXTERN uint64_t g_log_failed_posts; 
/*C number of failed logger POSTS (might have succeeded on retry */

/*C allow some retries. If a post makes 2 attempts and fails on both,
then num_bad_posts is incremented by 1 and num_failed_posts by 2. If
it succeeds on second attempt, then num_failed_posts increments by 1
and num_bad_posts stays as is
*/

EXTERN uint64_t g_log_bad_uuid; 
/*C TODO */
EXTERN uint64_t g_log_bad_test_type;
/*C TODO */
EXTERN uint64_t g_log_bad_test_name;
/*C TODO */
EXTERN uint64_t g_log_bad_tracer; 
/*C TODO */

EXTERN uint64_t g_log_get_alt_variant_calls; 
/*C TODO */
EXTERN uint64_t g_log_get_variant_calls; 
/*C TODO */
EXTERN uint64_t g_log_get_variants_calls; 
/*C TODO */
EXTERN uint64_t g_log_router_calls;
/*C TODO */
EXTERN uint64_t g_log_bad_router_calls;
/*C TODO */


