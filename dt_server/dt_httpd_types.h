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

#endif
