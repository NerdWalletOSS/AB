/* Lua uses this file to generate constants for itself
 * the format should always be #define<space>variable<space><value>
 * Note that the value is on a single line and has no spaces
 * Also only a single underscore should be used between two alphabets
 */

#ifndef __AB_CONSTANTS
#define __AB_CONSTANTS
#define TRUE 1
#define FALSE 0
#define AB_ERR_MSG_LEN 1023
#define AB_MAX_LEN_API_NAME 31
#define AB_MAX_LEN_ARGS     4095
#define AB_MAX_LEN_URL      4095 // used for URLs fired by RTS
#define AB_MAX_LEN_CUSTOM_DATA      2047 // 
#define AB_MAX_HEADERS_SIZE 8191
#define AB_MAX_LEN_BODY    32767
#define AB_MAX_LEN_PAYLOAD  1024-1 // for POST to logger
#define AB_MAX_LEN_RESULT   65535

#define AB_MAX_LEN_HDR_KEY 63
#define AB_MAX_LEN_HDR_VAL 127

#define AB_MAX_LEN_SS_RESPONSE 2047

#define AB_MIN_NUM_VARIANTS 2 // includes control as 1 of them
#define AB_MAX_NUM_VARIANTS 8 // includes control as 1 of them

#define AB_MAX_LEN_TEST_NAME    127
#define AB_MAX_LEN_VARIANT_NAME 31
#define AB_MAX_LEN_VARIANT_URL 255
#define AB_MAX_NUM_TESTS_IN_ONE_CALL 16

#define AB_NUM_BINS             1000 

#define NUM_SERVICES 7

#define TEST_STATE_DRAFT        1 
#define TEST_STATE_DORMANT      2 
#define TEST_STATE_STARTED      3 
#define TEST_STATE_TERMINATED   4 
#define TEST_STATE_ARCHIVED     5 

#define AB_MAX_LEN_SERVER_NAME 63

#define AB_MAX_NUM_TESTS 1024

#define AB_MAX_LEN_FILE_NAME 127

#define AB_MAX_LEN_DATE 63

#define AB_TEST_TYPE_AB 1
#define AB_TEST_TYPE_XY 2

#define AB_MAX_LEN_UUID     47 // Minimum value is 16
#define AB_GUID_LENGTH      8 // TO delete and replace by out tracer

#define AB_MAX_LEN_XY_ARGS 1023
#define AB_MAX_LEN_REDIRECT_URL (AB_MAX_LEN_VARIANT_URL+1+AB_MAX_LEN_XY_ARGS+1+128)
/* Leave some extra space for UUID and VariantID
    sprintf(g_redirect_url, "%s?%s&VariantID=%dGUID=%s", 
*/
#define AB_MAX_LEN_USER_AGENT 1024-1

#define AB_MAX_LEN_HOSTNAME 63
#define AB_MAX_LEN_DOMAIN   63

#define AB_MAX_LEN_TRACER 31

#define AB_DEFAULT_N_LOG_Q 65536

#define AB_MAX_LEN_LKP_NAME 31
#define AB_MAX_LEN_TEST_TYPE 15

#define AB_LOGGER_TIMEOUT_MS 1000
#define AB_SS_TIMEOUT_MS 50
    /* The get_or_create endpoint averages around 25ms response, with 
     * a 95th percentile of 30ms. The plain get endpoint would be 
     * expected to be faster -- Andrew Hollenbach */

#define AB_MAX_LEN_IP_ADDRESS 31
#define AB_MAX_LEN_POSTAL_CODE 7
#define AB_MAX_LEN_TIME_ZONE   63
#define AB_MAX_LEN_COUNTRY    3
#define AB_MAX_LEN_STATE      31
#define AB_MAX_LEN_CITY       31

#define AB_MAX_LEN_KAFKA_PARAM 63
#define AB_MAX_LEN_MYSQL_PARAM 63

#define AB_SEED_1 961748941 // large prime number
#define AB_SEED_2 982451653 // some other large primenumber


#define AB_ERROR_CODE_BAD_UUID -2
#define AB_ERROR_CODE_BAD_TEST -3

#define AB_MAX_LEN_STATSD_KEY 63
#define AB_MAX_LEN_STATSD_BUF 127 

#define  AB_LOGEVENT_VERSION_NUM 1 // Change VERY carefully!
#endif

