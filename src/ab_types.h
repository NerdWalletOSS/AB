#ifndef __ABTYPES_H
#define __ABTYPES_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <curl/curl.h>
#include "ab_constants.h"

typedef enum _req_type {
  Undefined,
  AddTest, // RW: pass to Lua
  CheckLoggerConnectivity, // RO: RTS 
  CheckDBConnectivity, // Config: pass to Lua
  DeleteTest, // RW: pass to Lua
  Diagnostics, // RO: RTS  AND Lua 
  DumpLog, // RO: RTS
  GetVariant, // RO: RTS
  GetVariants, // RO: RTS
  Halt, // RO: RTS
  HealthCheck, // RO: RTS
  Ignore, // RO: RTS
  ListTests, // RO: pass to Lua
  PingLogServer, // RO: RTS
  PingSessionServer, // RO: RTS
  Reload, // RO: RTS 
  Restart, // RO: RTS 
  Router, // RO: RTS
  SetPercentages, // RW: pass to Lua
  SetDevSpecPerc, // RW: pass to Lua
  StopTest, // RW: pass to Lua
  TestInfo, // RO: pass to Lua
  UAToDevice // RO: RTS
} AB_REQ_TYPE;

typedef struct _dev_spec_perc_type {
  uint32_t variant_idx;
  uint32_t final_variant_idx;
  uint32_t percentage;
  uint32_t cum_percentage;
} DEV_SPEC_PERC_TYPE;

typedef struct _variant_rec_type {
  uint32_t id;
  float percentage;
  char name[AB_MAX_LEN_VARIANT_NAME+1];
  char *url; // AB_MAX_LEN_VARIANT_URL+1
  char *custom_data; // AB_MAX_LEN_CUSTOM_DATA+1 
} VARIANT_REC_TYPE;

typedef struct _test_meta_type {
  char name[AB_MAX_LEN_TEST_NAME+1];
  int test_type; // whether AB_TEST_TYPE or XY_TEST_TYPE or ..
  uint32_t x_tst_id; // external test id
  uint64_t name_hash;
  uint64_t external_id; // exposed to external entities
  bool has_filters; // has filters using categorical/boolean attributes
  bool is_dev_specific; // whether device specific
  int state;   // one of TEST_STATE_....
  uint64_t seed; // seed for spooky hash 

  uint32_t num_variants; 
  VARIANT_REC_TYPE *variants;

  uint32_t final_variant_id; 
  uint32_t final_variant_idx; 

  uint8_t variant_per_bin[AB_NUM_BINS];

  DEV_SPEC_PERC_TYPE *dev_spec_perc; // [g_num_devices]
  uint32_t n_dev_spec_perc; // redundant since =  g_num_devices
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

#endif

