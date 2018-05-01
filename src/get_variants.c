#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <unistd.h>
#include "ab_constants.h"
#include "macros.h"
#include "ab_types.h"
#include "auxil.h"
#include "get_variant.h"
#include "get_variants.h"
#include "ab_globals.h"


int 
get_variants(
    const char *args
    )
{
  int status = 0;
  char test_name[AB_MAX_LEN_TEST_NAME+1];
#define N1 (AB_MAX_NUM_TESTS_IN_ONE_CALL*(AB_MAX_LEN_TEST_NAME+1))
#define N2 ((AB_MAX_LEN_TEST_NAME+1)+AB_MAX_LEN_UUID+64)
  char test_names[N1];
  char buf[N2];

  if ( ( args == NULL ) || ( *args == '\0' ) ) { go_BYE(-1); }
  // Extract UUID
  memset(g_uuid, '\0', g_cfg.uuid_len+1);
  status = extract_name_value(args, "UUID=", '&', g_uuid, g_cfg.uuid_len); 
  cBYE(status); // ADDED DEC 2016
  status = chk_uuid(g_uuid, g_cfg.uuid_len); cBYE(status);
  //-------------------------------------------------------------
  memset(test_names, '\0', N1);
  status = extract_name_value(args, "TestName=", '&', test_names, N1);
  cBYE(status);
  if ( test_names[0] == '\0' ) { g_log_no_test_names++; go_BYE(-1); }
  //-------------------------------------------------------------
  memset(g_mrslt, '\0', AB_MAX_LEN_RESULT+1);
  int oplen = 2; // length of output string 
  /* WILL NOT IMPLEMENT. Do not agree with Abhi's request below

Abhi: make sure that GetVariants returns a valid json. Right now it 
returns an array which is not a valid json string. Just make it 

{'Result':[....]}

instead of 
[....]
*/
  strcpy(g_mrslt, "[");
  char *cptr = test_names; 
  for ( int tidx = 0; ; tidx++ ) {
    int cidx = 0;
    memset(test_name, '\0', AB_MAX_LEN_TEST_NAME+1);
    for ( ; *cptr != '\0'; cptr++ ) { 
      if ( ( *cptr == ';' ) || ( *cptr == ',' ) ) { 
        if ( cidx == 0 ) { go_BYE(-1); } // empty test name 
        test_name[cidx] = '\0'; 
        cptr++; // jump over the semi-colon
        break;
      }
      else {
        if ( cidx >= AB_MAX_LEN_TEST_NAME ) { go_BYE(-1); }
        test_name[cidx++] = *cptr;
      }
    }
    if ( test_name[0] == '\0' ) { break; }
    memset(buf, '\0', N2);
    int nw = snprintf(buf, N2, "TestName=%s&UUID=%s&", test_name, g_uuid);
    if ( nw >= N2 ) { go_BYE(-1); }
    status = get_variant(buf);
    if ( status < 0 ) { 
      strcpy(g_rslt, "{ \"Variant\" : \"\", \"VariantID\" : 0, \"Test\" : \"\", \"TestID\" : 0, \"Ramp\" : 0, \"UsesEEE\" : 0 } ");
      status = 0;
    }
    if ( tidx > 0 ) { 
      oplen += 2; 
      if ( oplen >= AB_MAX_LEN_RESULT ) { go_BYE(-1); }
      strcat(g_mrslt, ", ");
    }
    oplen += strlen(g_rslt);
    if ( oplen >= AB_MAX_LEN_RESULT ) { go_BYE(-1); }
    strcat(g_mrslt, g_rslt);
  }
  oplen += 2; 
  if ( oplen >= AB_MAX_LEN_RESULT ) { go_BYE(-1); }
  strcat(g_mrslt, "]");
  strcpy(g_rslt, g_mrslt);
BYE:
  return status;
}
