#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "get_req_type.h"
AB_REQ_TYPE
get_req_type(
    const char *api
    )
{
  if (strcasecmp(api, "AddTest") == 0) {
    return AddTest;
  }
  else if (strcasecmp(api, "CheckLoggerConnectivity") == 0) {
    return CheckLoggerConnectivity;
  }
  else if (strcasecmp(api, "CheckLoggerConnectivity") == 0) {
    return CheckDBConnectivity;
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
  else if (strcasecmp(api, "GetVariant") == 0) {
    return GetVariant;
  }
  else if (strcasecmp(api, "GetVariants") == 0) {
    return GetVariants;
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
  else if (strcasecmp(api, "ListTests") == 0) {
    return ListTests;
  }
  else if (strcasecmp(api, "LoadConfig") == 0) {
    return LoadConfig;
  }
  else if (strcasecmp(api, "PingLogServer") == 0) {
    return PingLogServer;
  }
  else if (strcasecmp(api, "PingSessionServer") == 0) {
    return PingSessionServer;
  }
  else if (strcasecmp(api, "Reload") == 0) {
    return Reload;
  }
  else if (strcasecmp(api, "Restart") == 0) {
    return Restart;
  }
  else if (strcasecmp(api, "Router") == 0) {
    return Router;
  }
  else if (strcasecmp(api, "TestInfo") == 0) {
    return TestInfo;
  }
  else if (strcasecmp(api, "UAToDevice") == 0) {
    return UAToDevice;
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
