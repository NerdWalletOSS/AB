#include <stdio.h>
#include "ab_constants.h"
#include "ab_types.h"
#include "macros.h"
#include "ab_globals.h"
#include "auxil.h"
#include "make_curl_payload.h"

//<hdr>
void
zero_payload(
    PAYLOAD_TYPE *ptr_payload
    )
{
  memset(ptr_payload->uuid, '\0', AB_MAX_LEN_UUID+1);
  memset(ptr_payload->in_tracer, '\0', AB_MAX_LEN_TRACER+1);
  memset(ptr_payload->out_tracer, '\0', AB_MAX_LEN_TRACER+1);
  ptr_payload->ramp = 0;
  ptr_payload->time = 0;
  ptr_payload->test_id = 0;
  ptr_payload->variant_id = 0;
}

//</hdr>
/* Reads lcl_payload and writes to a string (curl_payload) that 
 will be POSTed to log server */
//<hdr>
int 
make_curl_payload(
    PAYLOAD_TYPE lcl_payload,
    char *curl_payload,
    size_t sz_curl_payload
    )
//</hdr>
{
  int status = 0;

  memset(curl_payload,            '\0', sz_curl_payload+1);
  size_t sz = snprintf(curl_payload, sz_curl_payload, 
"{  \
\"uuid\" : \"%s\", \
\"test_id\" : \"%u\", \
\"variant_id\" : \"%u\", \
\"ramp\" : \"%u\", \
\"in_tracer_id\" : \"%s\", \
\"out_tracer_id\" : \"%s\", \
\"time\" : \"%" PRIu64 "\" }",
      lcl_payload.uuid,
      lcl_payload.test_id, 
      lcl_payload.variant_id,
      lcl_payload.ramp,
      lcl_payload.in_tracer,
      lcl_payload.out_tracer,
      lcl_payload.time
      ); 
  if ( sz >= AB_MAX_LEN_PAYLOAD ) { go_BYE(-1); }
  // TODO Log buffer overflow
BYE:
  return status;
}
/*START_COMMENT
message ABLog {
    required string uuid = 1;
    required uint32 test_id = 2;
    required uint32 variant_id = 3;
    required uint64 time = 4;
    required uint32 ramp = 5;
    optional string ip = 6;
    optional string in_tracer_id = 7;  
// An id that can be passed around server-side to connect events 
// that are not directly related (i.e. with offline models and AB testing)
    optional string out_tracer_id = 8;  
// An id that can be passed around server-side to connect events 
// that are not directly related (i.e. with offline models and AB testing)
}
STOP_COMMENT*/
