#ifndef __DT_HTTPD_TYPES_H
#define __DT_HTTPD_TYPES_H
#include "dt_incs.h"

typedef enum _dt_req_type {
  Undefined, // --- & ---
  Classify, // Read & C 
  Diagnostics, // Read &  C  AND Lua
  DumpLog, // Read &  C
  GetConfig, // Read &  Lua
  GetNumFeatures, // Read &  Lua
  Halt, // Read &  C
  HealthCheck, // Read &  C
  Ignore, // Read &  C
  LoadModels, // Write &  Lua TODO 
  MakeFeatureVector, // Read &  Lua
  MdlMeta, // Read & Lua 
  PostProcPreds, // Read &  C
  Restart, // Read &  C
  ZeroCounters // Write &  C
} DT_REQ_TYPE;


typedef struct _cfg_type {

  char dt_dir[DT_MAX_LEN_FILE_NAME+1]; // For decision tree
  char model_name[DT_MAX_LEN_FILE_NAME+1]; // For decision tree

  int32_t  port;  // port on which AB RTS will run
  bool verbose;    // how chatty should RTS be

} CFG_TYPE;

#endif
