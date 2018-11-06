#include "dt_incs.h"
#include "dt_httpd_types.h"
#include "auxil.h"
#include "get_req_type.h"
DT_REQ_TYPE
get_req_type(
    const char *api
    )
{
  if (strcasecmp(api, "Classify") == 0) {
    return Classify;
  }
  else if (strcasecmp(api, "Diagnostics") == 0) {
    return Diagnostics;
  }
  else if (strcasecmp(api, "DumpLog") == 0) {
    return DumpLog;
  }
  else if (strcasecmp(api, "GetConfig") == 0) {
    return GetConfig;
  }
  else if (strcasecmp(api, "GetNumFeatures") == 0) {
    return GetNumFeatures;
  }
  else if (strcasecmp(api, "Halt") == 0) {
    return Halt;
  }
  else if (strcasecmp(api, "HealthCheck") == 0) {
    return HealthCheck;
  }
  else if (strcasecmp(api, "Ignore") == 0) {
    return Ignore;
  }
  else if (strcasecmp(api, "LoadModels") == 0) {
    return LoadModels;
  }
  else if (strcasecmp(api, "MakeFeatureVector") == 0) {
    return MakeFeatureVector;
  }
  else if (strcasecmp(api, "MdlMeta") == 0) {
    return MdlMeta;
  }
  else if (strcasecmp(api, "PostProcPreds") == 0) {
    return PostProcPreds;
  }
  else if (strcasecmp(api, "Restart") == 0) {
    return Restart;
  }
  else if (strcasecmp(api, "ZeroCounters") == 0) {
    return ZeroCounters;
  }
  else if ( strcasecmp(api, "favicon.ico") == 0 ) { 
    return Ignore;
  }
  else {
    fprintf(stderr,  "Unknown API = %s \n", api);

    return Undefined;
  }
  return Undefined;
}
