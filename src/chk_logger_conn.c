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
  PAYLOAD_TYPE lcl_payload;

  if ( nX < AB_MAX_LEN_PAYLOAD+1+32 ) { go_BYE(-1); }
  if ( g_ch == NULL ) { 
    strcpy(X, "{ \"ChkPipe\" : \"Logging Disabled\" } ");
    goto BYE;
  }
  memset(curl_payload, '\0', AB_MAX_LEN_PAYLOAD+1);
  memset(&lcl_payload, '\0', sizeof(PAYLOAD_TYPE));
  // START: Make a bogus UUID that is alphanumeric 
  strcpy(lcl_payload.uuid,   "CHCKPIPE");
  strcpy(lcl_payload.in_tracer, "CHCKPIPE");
  strcpy(lcl_payload.out_tracer, "CHCKPIPE");
  //----------------------------------------------
  if ( lcl_payload.uuid == 0 ) { go_BYE(-1); }
  status = make_curl_payload(lcl_payload, curl_payload); cBYE(status);
  status = post_url(g_ch, curl_payload, &time_taken);
  if ( strlen(curl_payload) >= nX ) { go_BYE(-1); }
  if ( status < 0 ) { 
    nw = snprintf(X, nX,
        "{ \"ChkPipe\" : \"ERROR:%s\" } ", curl_payload);
    if ( nw >= nX ) { go_BYE(-1); }
    fprintf(stderr, "%u:%s:%u Line %3d of File %s, Payload = %s \n",
        get_time_sec(), g_my_name, g_port, __LINE__, __FILE__, curl_payload);
  }
  else {
    nw = snprintf(X, nX, 
        "{ \"ChkPipe\" : \"%" PRIu64 "\", \"GUID\" : \"%s\" }", 
        time_taken, lcl_payload.uuid);
    if ( nw >= nX ) { go_BYE(-1); }
  }
BYE:
  return status;
}
