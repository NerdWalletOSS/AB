#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "post_url.h"
#include "chk_logger_conn.h"
#include "make_curl_payload.h"

int chk_logger_connectivity(
    char *X,
    size_t nX
    )
{
  int status = 0;
  uint64_t time_taken = 0; size_t nw;
  char curl_payload[AB_MAX_LEN_PAYLOAD+1];
  PAYLOAD_REC_TYPE lcl_payload;

  if ( nX < AB_MAX_LEN_PAYLOAD+1+32 ) { go_BYE(-1); }
  if ( g_ch == NULL ) { 
    strcpy(X, "{ \"CheckLoggerConnectivity\" : \"Logging Disabled\" } ");
    goto BYE;
  }
  memset(&lcl_payload, '\0', sizeof(PAYLOAD_REC_TYPE));
  // START: Make a bogus UUID that is alphanumeric 
  strncpy(lcl_payload.uuid, "UUID_CHCKPIPE", g_cfg.max_len_uuid);
  strncpy(lcl_payload.in_tracer, "IN_CHCKPIPE", AB_MAX_LEN_TRACER);
  strncpy(lcl_payload.out_tracer, "OUT_CHCKPIPE", AB_MAX_LEN_TRACER);
  //----------------------------------------------
  status = make_curl_payload(lcl_payload, curl_payload, AB_MAX_LEN_PAYLOAD); cBYE(status);
  status = post_url(g_ch, curl_payload, &time_taken);
  if ( status < 0 ) { 
    nw = snprintf(X, nX,
        "{ \"CheckLoggerConnectivity\" : \"ERROR:%s\" } ", curl_payload);
    if ( nw >= nX ) { go_BYE(-1); }
    status = 0;
  }
  else {
    nw = snprintf(X, nX, 
        "{ \"CheckLoggerConnectivity\" : \"%" PRIu64 "\", \"GUID\" : \"%s\" }", 
        time_taken, lcl_payload.uuid);
    if ( nw >= nX ) { go_BYE(-1); }
  }
BYE:
  return status;
}
