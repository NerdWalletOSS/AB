#ifndef __ABTYPES_H
#define __ABTYPES_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <curl/curl.h>
#include "ab_constants.h"

typedef enum _ab_req_type {
  Undefined, // --- & --- 
  AddTest, // Write &  Lua
  CheckLoggerConnectivity, // Read &  C 
  CheckDBConnectivity, // Config &  Lua
  ClassifyIP, // Read &  C
  ClassifyUA, // Read &  C
  Diagnostics, // Read &  C  AND Lua 
  DumpLog, // Read &  C
  EvalDT, // Read & C
  GetConfig, // Read &  Lua
  GetVariant, // Read &  C
  GetVariants, // Read &  C
  Halt, // Read &  C
  HealthCheck, // Read &  C
  Ignore, // Read &  C
  ListTests, // Read &  Lua
  LoadConfig, // Write &  Lua
  PingLogServer, // Read &  C
  PingSessionServer, // Read &  C
  Reload, // Write &  Lua 
  Restart, // Read &  C 
  Router, // Read &  C
  TestInfo, // Read &  Lua
  ZeroCounters // Write &  C
} AB_REQ_TYPE;

typedef struct _variant_rec_type {
  uint32_t id;
  float percentage;
  char name[AB_MAX_LEN_VARIANT_NAME+1];
  char * url; // AB_MAX_LEN_VARIANT_URL+1
  char * custom_data; // AB_MAX_LEN_CUSTOM_DATA+1 
} VARIANT_REC_TYPE;

typedef struct _test_meta_type {
  char name[AB_MAX_LEN_TEST_NAME+1];
  int test_type; // whether AB_TEST_TYPE or XY_TEST_TYPE or ..
  uint32_t id; // external test id
  uint64_t name_hash; // set by Lua, read by C 
  uint64_t external_id; // exposed to external entities
  bool has_filters; // has filters using categorical/boolean attributes
  bool is_dev_specific; // whether device specific
  int state;   // one of TEST_STATE_....
  uint64_t seed; // seed for spooky hash 

  // Following are set by Lua, read by C 
  uint32_t num_variants;  
  VARIANT_REC_TYPE *variants;

  // If device specific is not set, we use device_idx = 0
  uint32_t *final_variant_id; // [g_num_devices]; 
  uint32_t *final_variant_idx; // [g_num_devices]; 
  uint8_t *variant_per_bin[AB_NUM_BINS]; // [g_num_devices][AB_NUM_BINS]; 

} TEST_META_TYPE;

typedef struct _payload_type {
  char uuid[AB_MAX_LEN_UUID+1];
  char in_tracer[AB_MAX_LEN_TRACER+1];
  char out_tracer[AB_MAX_LEN_TRACER+1];
  uint64_t time;
  uint32_t test_id;
  uint32_t variant_id;
} PAYLOAD_TYPE;

typedef struct _lkp_rec_type { 
  uint32_t id;
  char name[AB_MAX_LEN_LKP_NAME+1];
} LKP_REC_TYPE;

typedef struct _ua_rec_type{
  uint32_t top_hash;
  uint32_t bot_hash;
  uint8_t device_type_id;
  uint8_t os_id;
  uint8_t browser_id;
  uint8_t justin_cat_id;
} UA_REC_TYPE;

typedef struct _service_type {
  uint16_t  port; 
  char server[AB_MAX_LEN_SERVER_NAME+1]; 
  char url[AB_MAX_LEN_URL+1]; 
  char health_url[AB_MAX_LEN_URL+1]; 
} SERVICE_TYPE;

typedef struct _cftype {

  uint16_t  port;  // port on which AB RTS will run 
  bool verbose;    // how chatty should RTS be 
  
  SERVICE_TYPE logger;
  SERVICE_TYPE ss;
  SERVICE_TYPE statsd;

  uint32_t sz_log_q;
  int num_post_retries;  
  
  char default_url[AB_MAX_LEN_REDIRECT_URL+1]; 

  int uuid_len; 
  uint64_t xy_guid; // Set to 0 for real, positive integer for testing

  // START: For classifying user agent 
  char ua_to_dev_map_file[AB_MAX_LEN_FILE_NAME+1]; 
  char justin_cat_file[AB_MAX_LEN_FILE_NAME+1]; 
  char os_file[AB_MAX_LEN_FILE_NAME+1]; 
  char browser_file[AB_MAX_LEN_FILE_NAME+1]; 
  char device_type_file[AB_MAX_LEN_FILE_NAME+1]; 
  // STOP: For classifying user agent 
  // START: For ML 
  char avg_fico_per_zip_file[AB_MAX_LEN_FILE_NAME+1]; // For Lua
  char avg_zill_per_zip_file[AB_MAX_LEN_FILE_NAME+1]; // For Lua
  char referer_class_file[AB_MAX_LEN_FILE_NAME+1]; 
  // STOP: For ML 
  // START: For decision tree
  char dt_feature_file[AB_MAX_LEN_FILE_NAME+1]; // For Lua 
  char dt_file[AB_MAX_LEN_FILE_NAME+1]; 
  int  dt_n_features;
  // STOP : For decision tree
  char mmdb_file[AB_MAX_LEN_FILE_NAME+1];  // For MaxMind
} CFG_TYPE;

typedef struct _justin_map_rec_type { 
  char justin_cat[AB_MAX_LEN_LKP_NAME+1];
  char device[AB_MAX_LEN_LKP_NAME+1];
  char device_type[AB_MAX_LEN_LKP_NAME+1];
  char os[AB_MAX_LEN_LKP_NAME+1];
  char browser[AB_MAX_LEN_LKP_NAME+1];
} JUSTIN_MAP_REC_TYPE;

typedef struct _maxmind_rec_type { 
  char postal_code[AB_MAX_LEN_POSTAL_CODE+1]; // "postal", "code"
  char time_zone[AB_MAX_LEN_TIME_ZONE+1]; // "location", "time_zone"
  char country[4]; // "country", "iso_code"
  char state[AB_MAX_LEN_STATE+1]; // "subdivisions", "names", "en"
  char city[AB_MAX_LEN_CITY+1]; // "city", "names", "en"
} MAXMIND_REC_TYPE;

#endif

