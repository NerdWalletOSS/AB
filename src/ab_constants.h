#ifndef __AB_CONSTANTS
#define __AB_CONSTANTS
#define TRUE 1
#define FALSE 0
#define AB_ERR_MSG_LEN 1023
#define SERVER_REQ_Q 128 // number of concurrent requests 
#define AB_MAX_LEN_API_NAME 31
#define AB_MAX_LEN_ARGS     4095
#define AB_MAX_LEN_URL      4095 // used for URLs fired by RTS
#define AB_MAX_HEADERS_SIZE 8191
#define AB_MAX_LEN_BODY    8191
#define AB_MAX_LEN_PAYLOAD  1024-1 // for POST to logger
#define AB_MAX_LEN_RESULT   1048576-1

#define AB_MAX_LEN_SS_RESPONSE 2047

#define AB_MIN_NUM_VARIANTS 2 // includes control as 1 of them
#define AB_MAX_NUM_VARIANTS 8 // includes control as 1 of them

#define AB_MAX_LEN_TEST_NAME    127
#define AB_MAX_LEN_VARIANT_NAME 31
#define AB_MAX_LEN_VARIANT_URL 255
#define AB_MAX_NUM_TESTS_IN_ONE_CALL 16

#define AB_NUM_BINS             32749 // TODO THINK ABOUT THIS

#define TEST_STATE_DRAFT        1 
#define TEST_STATE_DORMANT      2 
#define TEST_STATE_STARTED      3 
#define TEST_STATE_TERMINATED   4 
#define TEST_STATE_ARCHIVED     5 

#define AB_MAX_LEN_SERVER_NAME 63

#define AB_MAX_NUM_TESTS 1024

#define AB_MAX_LEN_FILE_NAME 127

#define AB_TEST_TYPE_AB 1
#define AB_TEST_TYPE_XY 2

#define AB_MAX_LEN_UUID     63 // Minimum value is 16
#define AB_GUID_LENGTH      8 // TO delete and replace by out tracer

#define AB_MAX_LEN_XY_ARGS 1023
#define AB_MAX_LEN_REDIRECT_URL \
  (AB_MAX_LEN_VARIANT_URL+1+AB_MAX_LEN_XY_ARGS+1+128)
/* Leave some extra space for UUID and VariantID
    sprintf(g_redirect_url, "%s?%s&VariantID=%dGUID=%s", 
*/
#define AB_MAX_LEN_USER_AGENT 1024-1

#define AB_MAX_LEN_HOSTNAME 63

#define AB_MAX_LEN_TRACER 63

#define AB_DEFAULT_N_LOG_Q 128

#define AB_MAX_LEN_DEVICE 15
#define AB_MAX_LEN_TEST_TYPE 15
#endif
