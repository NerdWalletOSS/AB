#ifndef __AB_CONSTANTS
#define __AB_CONSTANTS
#define TRUE 1
#define FALSE 0
#define AB_ERR_MSG_LEN 1023
#define SERVER_REQ_Q 128 // number of concurrent requests 
#define AB_MAX_LEN_API_NAME 31
#define AB_MAX_LEN_ARGS     4095
#define AB_MAX_LEN_URL      4095 // used for URLs fired by RTS
#define AB_MAX_LEN_POST     8191
#define AB_MAX_HEADERS_SIZE 8191
#define AB_MAX_BODY_SIZE    AB_MAX_LEN_POST
#define AB_MAX_LEN_PAYLOAD  1023 // for POST to logger
#define AB_MAX_LEN_RESULT   1048575

#define AB_MAX_LEN_SS_RESPONSE 2047

#define AB_MIN_NUM_VARIANTS 2 // includes control as 1 of them
#define AB_MAX_NUM_VARIANTS 8 // includes control as 1 of them

#define AB_MAX_LEN_NAME_LKP_TBL 15
#define AB_MAX_LEN_TEST_TYPE     7
#define AB_MAX_LEN_STATE_NAME   15

#define AB_MAX_LEN_TEST_NAME    127
#define AB_MAX_LEN_VARIANT_NAME 31
#define AB_MAX_LEN_VARIANT_URL 255
#define AB_MAX_NUM_TESTS_IN_ONE_CALL 16

#define AB_NUM_BINS             32749

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

#define AB_MAX_LEN_UUID     63
#define AB_GUID_LENGTH      8 // TO delete and replace by out tracer

#define AB_MAX_LEN_XY_ARGS 1023
#define AB_MAX_LEN_REDIRECT_URL \
  (AB_MAX_LEN_VARIANT_URL+1+AB_MAX_LEN_XY_ARGS+1+128)
/* Leave some extra space for UUID and VariantID
    sprintf(g_redirect_url, "%s?%s&VariantID=%dGUID=%s", 
*/
#define AB_MAX_LEN_USER_AGENT 2047

#define AB_MAX_LEN_CAT_ATTR     15
#define AB_MAX_LEN_CAT_ATTR_VAL 15
#define AB_MAX_LEN_BOOL_ATTR     AB_MAX_LEN_CAT_ATTR     

#define AB_MAX_NUM_CAT_ATTR     16
#define AB_MAX_NUM_VALS_PER_ATTR 16

#define AB_SZ_CAT_ATTR_TEST (AB_MAX_NUM_TESTS* AB_MAX_NUM_CAT_ATTR)
#define AB_SZ_CAT_ATTR_VAL_TEST (AB_MAX_NUM_TESTS* AB_MAX_NUM_CAT_ATTR*AB_MAX_NUM_VALS_PER_ATTR)
#define AB_SZ_BOOL_ATTR_VAL_TEST AB_MAX_NUM_TESTS

#define AB_MAX_LEN_HOSTNAME 63

#define AB_MAX_NUM_DEVICES 8
#define AB_MAX_LEN_DEVICE  15

#define AB_MAX_DIGITS_IN_INT64 20
#define AB_MAX_DIGITS_IN_INT32 20

#define AB_MAX_LEN_CUSTOM_DATA 2047

#define AB_MAX_LEN_TRACER 63

#define AB_DEFAULT_N_LOG_Q 128
#endif
