#include "ab_incs.h"
#include "auxil.h"
#include "ab_globals.h"
#include "get_req_type.h"
AB_REQ_TYPE
get_req_type(
    const char *api
    )
{
  if (strcasecmp(api, "AddFakeTest") == 0) {
    return AddFakeTest;
  }
  else if (strcasecmp(api, "AddTest") == 0) {
    return AddTest;
  }
  else if (strcasecmp(api, "CheckLoggerConnectivity") == 0) {
    return CheckLoggerConnectivity;
  }
  else if (strcasecmp(api, "CheckLoggerConnectivity") == 0) {
    return CheckDBConnectivity;
  }
  else if (strcasecmp(api, "CheckTest") == 0) {
    return CheckTest;
  }
  else if (strcasecmp(api, "Classify") == 0) {
    return Classify;
  }
  else if (strcasecmp(api, "ClassifyIP") == 0) {
    return ClassifyIP;
  }
  else if (strcasecmp(api, "ClassifyUA") == 0) {
    return ClassifyUA;
  }
  else if (strcasecmp(api, "DeleteTest") == 0) {
    return DeleteTest;
  }
  else if (strcasecmp(api, "Diagnostics") == 0) {
    return Diagnostics;
  }
  else if (strcasecmp(api, "DumpLog") == 0) {
    return DumpLog;
  }
  else if (strcasecmp(api, "EvalDT") == 0) {
    return EvalDT;
  }
  else if (strcasecmp(api, "GetConfig") == 0) {
    return GetConfig;
  }
  else if (strcasecmp(api, "GetDomain") == 0) {
    return GetDomain;
  }
  else if (strcasecmp(api, "GetHost") == 0) {
    return GetHost;
  }
  else if (strcasecmp(api, "GetNumFeatures") == 0) {
    return GetNumFeatures;
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
  else if (strcasecmp(api, "IgnoreKafkaErrors") == 0) {
    return IgnoreKafkaErrors;
  }
  else if (strcasecmp(api, "ListTests") == 0) {
    return ListTests;
  }
  else if (strcasecmp(api, "LoadConfig") == 0) {
    return LoadConfig;
  }
  else if (strcasecmp(api, "MakeFeatureVector") == 0) {
    return MakeFeatureVector;
  }
  else if (strcasecmp(api, "MdlMeta") == 0) {
    return MdlMeta;
  }
  else if (strcasecmp(api, "PingServer") == 0) {
    return PingServer;
  }
  else if (strcasecmp(api, "PostProcPreds") == 0) {
    return PostProcPreds;
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
  else if (strcasecmp(api, "StopTest") == 0) {
    return StopTest;
  }
  else if (strcasecmp(api, "TestInfo") == 0) {
    return TestInfo;
  }
  else if (strcasecmp(api, "ToKafka") == 0) {
    return ToKafka;
  }
  else if (strcasecmp(api, "UTMKV") == 0) {
    return UTMKV;
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
