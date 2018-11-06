#ifndef __DT_HTTPD_TYPES_H
#define __DT_HTTPD_TYPES_H
#include "dt_incs.h"

typedef enum _dt_req_type {
  Undefined, // --- & ---
  AddFakeTest, // Read & C (for testing)
  AddTest, // Write &  Lua
  CheckLoggerConnectivity, // Read &  C
  CheckKafkaConnectivity, // Read &  C
  CheckDBConnectivity, // Config &  Lua
  Classify, // Read & C 
  Diagnostics, // Read &  C  AND Lua
  DumpLog, // Read &  C
  EvalDT, // Read & C
  GetConfig, // Read &  Lua
  GetDomain, // Debugging &  Lua
  GetHost, // Debugging &  Lua
  GetNumFeatures, // Read &  Lua
  GetVariant, // Read &  C
  GetVariants, // Read &  C
  Halt, // Read &  C
  HealthCheck, // Read &  C
  Ignore, // Read &  C
  IgnoreKafkaErrors, // Read &  C
  ListTests, // Read &  Lua
  LoadConfig, // Write &  Lua
  MakeFeatureVector, // Read &  Lua
  MdlMeta, // Read & Lua 
  NumTests, // Read &  Lua
  PingServer, // Read &  C
  PostProcPreds, // Read &  C
  Reload, // Write &  Lua
  Restart, // Read &  C
  Router, // Read &  C
  StopTest, // Write & C (for testing)
  TestInfo, // Read &  Lua
  ToKafka, // Pass through & C
  UTMKV, // Read &  C
  ZeroCounters // Write &  C
} DT_REQ_TYPE;


typedef struct _cfg_type {

  char dt_dir[DT_MAX_LEN_FILE_NAME+1]; // For decision tree
  char model_name[DT_MAX_LEN_FILE_NAME+1]; // For decision tree

  int32_t  port;  // port on which AB RTS will run
  bool verbose;    // how chatty should RTS be

} CFG_TYPE;

#endif
