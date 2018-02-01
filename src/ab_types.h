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
  DeleteTest, // Write &  Lua
  Diagnostics, // Read &  C  AND Lua 
  DumpLog, // Read &  C
  GetVariant, // Read &  C
  GetVariants, // Read &  C
  Halt, // Read &  C
  HealthCheck, // Read &  C
  Ignore, // Read &  C
  ListTests, // Read &  Lua
  PingLogServer, // Read &  C
  PingSessionServer, // Read &  C
  Reload, // Write &  Lua 
  Restart, // Read &  C 
  Router, // Read &  C
  SetPercentages, // Write &  Lua
  SetDevSpecPerc, // Write &  Lua
  StopTest, // Write &  Lua
  TestInfo, // Read &  Lua
  UAToDevice, // Read &  C
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

typedef struct _dev_rec_type { 
  uint32_t id;
  char name[AB_MAX_LEN_DEVICE+1];
} DEV_REC_TYPE;

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

  char ua_to_dev_map_file[AB_MAX_LEN_FILE_NAME+1]; 

  uint32_t num_devices;
  char *devices[AB_MAX_LEN_DEVICE+1];
} CFG_TYPE;

#endif

