#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "ab_constants.h"
#include "macros.h"
#include "ab_types.h"
#include "auxil.h"
#include "url.h"
#include "spooky_hash.h"

extern UA_REC_TYPE *g_classify_ua_map; // Set by C
extern uint32_t g_num_classify_ua_map; // Set by C

extern uint32_t g_justin_cat_id; 
extern uint32_t g_os_id; 
extern uint32_t g_browser_id; 
extern uint32_t g_device_type_id; 

extern LKP_REC_TYPE *g_justin_cat_lkp; 
extern int g_n_justin_cat_lkp; 

extern LKP_REC_TYPE *g_os_lkp; 
extern int g_n_os_lkp; 

extern LKP_REC_TYPE *g_browser_lkp; 
extern int g_n_browser_lkp; 

extern LKP_REC_TYPE *g_device_type_lkp; 
extern int g_n_device_type_lkp; 
extern char g_valid_chars_in_url[256];
extern LKP_REC_TYPE *g_justin_cat_lkp;
extern int g_n_justin_cat_lkp; 
extern char g_valid_chars_in_ua[256]; 
extern char g_valid_chars_in_ab_args[256]; 

#include "classify_ua.h"
#include "load_lkp.h"


//<hdr>
int
ext_classify_ua(
    const char *args,
    char *X,
    size_t nX
    )
//</hdr>
{
  int status = 0;
  char in_user_agent[AB_MAX_LEN_USER_AGENT+1];
  uint32_t nw;
  char *user_agent = NULL;


  if ( ( X == NULL ) && ( nX == 0 ) ) { go_BYE(-1); }
  memset(in_user_agent, '\0', AB_MAX_LEN_USER_AGENT+1);
  status = extract_name_value(args, "UserAgent=", '&', 
      in_user_agent, AB_MAX_LEN_USER_AGENT);
  if ( in_user_agent[0] == '\0' ) { go_BYE(-1); }
  status = url_decode(in_user_agent, &user_agent); cBYE(status);
  status = validate_chars(user_agent, g_valid_chars_in_ua); cBYE(status);
  status = classify_ua(user_agent, g_classify_ua_map, 
      g_num_classify_ua_map, 
      &g_device_type_id, &g_os_id, &g_browser_id, &g_justin_cat_id);

  const char *justin_cat = lkp_id_to_name(g_justin_cat_lkp, 
      g_n_justin_cat_lkp, g_justin_cat_id);
  if ( justin_cat == NULL ) { justin_cat = "Unknown"; }

  const char *os = lkp_id_to_name(g_os_lkp, g_n_os_lkp, g_os_id); 
  if ( os == NULL ) { os = "Unknown"; }

  const char *browser = lkp_id_to_name(g_browser_lkp, g_n_browser_lkp, 
      g_browser_id); 
  if ( browser == NULL ) { browser = "Unknown"; }

  const char *device_type = lkp_id_to_name(
      g_device_type_lkp, g_n_device_type_lkp, g_device_type_id); 
  if ( device_type == NULL ) { device_type = "Unknown"; }

  nw = snprintf(X, nX, "{  \
      \"JustinCat\": \"%s\", \"JustinCatID\" : \"%u\", \
      \"OS\": \"%s\", \"OSID\" : \"%u\", \
      \"Browser\": \"%s\", \"BrowserID\" : \"%u\", \
      \"DeviceType\": \"%s\", \"DeviceTypeID\" : \"%u\" }",
      justin_cat, g_justin_cat_id,
      os, g_os_id,
      browser, g_browser_id,
      device_type, g_device_type_id);
  if ( nw >= nX ) { go_BYE(-1); }
BYE:
  free_if_non_null(user_agent);
  return status;
}

int
classify_ua(
    char *user_agent, 
    UA_REC_TYPE *X, // classify_ua_map
    uint32_t nX,  // num_classify_ua_map
    uint32_t *ptr_device_type_id,
    uint32_t *ptr_os_id,
    uint32_t *ptr_browser_id,
    uint32_t *ptr_justin_cat_id
    )
{
  int status = 0;
  *ptr_device_type_id = 0;
  *ptr_os_id = 0;
  *ptr_browser_id = 0;
  *ptr_justin_cat_id = 0;
  if ( ( user_agent == NULL ) || ( *user_agent == '\0' ) ) { go_BYE(-1); }
  if ( ( X == NULL ) || ( nX == 0 ) )  { go_BYE(-1); }
  char alnum_ua[AB_MAX_LEN_USER_AGENT+1];
  //---------------------------------
  // reduce user agent to alphanumeric characters, lower case
  memset(alnum_ua, '\0', AB_MAX_LEN_USER_AGENT+1);
  int idx = 0;
  for ( char *cptr = user_agent; *cptr != '\0'; cptr++ ) { 
    if ( isalnum(*cptr) ) {
      alnum_ua[idx++] = tolower(*cptr);
    }
  }
  int len_alnum_ua = idx;
  //---------------------------------

  uint64_t hash1 = 0, hash2 = 0;
  spooky_hash128(alnum_ua, len_alnum_ua, &hash1, &hash2);
  // Note: We must use hash1 because that is what process_user_agent does
  int mid, lb = 0, ub = nX; bool found = false;
  while ( !found ) { 
    mid = (lb + ub) / 2;
    UA_REC_TYPE *x1 = &(X[mid]);
    uint64_t this_hash = ((uint64_t *)x1)[0];
    if ( this_hash == hash1 ) {
      found = true; 
      *ptr_os_id = X[mid].os_id;
      *ptr_browser_id = X[mid].browser_id;
      *ptr_device_type_id = X[mid].device_type_id;
      *ptr_justin_cat_id = X[mid].justin_cat_id;
    }
    else if ( this_hash > hash1 ) { // TODO P1 Check comparison
      ub = mid;
    }
    else {
      lb = mid;
    }
    if ( mid == (lb+ub)/2 ) { break; }
    mid = (lb+ub)/2;
  }
BYE:
  return status;
}
