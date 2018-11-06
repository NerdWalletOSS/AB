#ifdef __DT_MAIN_PROGRAM
#define EXTERN extern
#else
#define EXTERN 
#endif
//--- For Internal log. All set by C

EXTERN uint64_t g_log_start_time;
/*C time (micro-seconds since epoch) at which server was started */
EXTERN uint64_t g_log_response_time; 
/*C Time taken to respond to /Classify */
EXTERN uint64_t g_log_num_classify; 
/*C Number of calls to /Classify */

