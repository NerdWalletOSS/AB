#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#include "ab_constants.h"
#include "macros.h"
#include "ab_types.h"
#include "auxil.h"
#include "post_url.h"

#include "ab_globals.h"

int
post_url(
    CURL *ch,
    const char *str_payload,
    uint64_t *ptr_time_taken
    )
{
  int status = 0; 
  CURLcode curl_res;
  uint64_t t_start, t_stop;

  if ( ch == NULL ) { goto BYE; } // Log Server Not running
  long http_code;
  // ----------------------------------------
  /* Now specify the POST data */ 
  g_log_posts++;
  curl_easy_setopt(ch, CURLOPT_POSTFIELDS, str_payload);
  if ( ptr_time_taken != NULL ) { t_start = get_time_usec(); }
  int num_tries = 0;
  bool good_post;
  do { 
    good_post = true;
    curl_res = curl_easy_perform(ch);
    num_tries++;
    if ( curl_res != CURLE_OK ) { 
      g_log_failed_posts++; good_post = false; 
    }
    else {
      curl_easy_getinfo(ch, CURLINFO_RESPONSE_CODE, &http_code);
      if ( http_code != 200 )  {  
        g_log_failed_posts++; good_post = false; 
      }
    }
    if ( good_post ) { break;}
  } while ( num_tries < g_num_post_retries );
  if ( !good_post ) { 
    g_log_bad_posts++; status = -2; 
  }
  // Note that the return is -2, not -1 which will be treated 
  // differently by caller
  if ( ptr_time_taken != NULL ) { 
    t_stop = get_time_usec();
    *ptr_time_taken = t_stop - t_start;
  }
BYE:
  return status;
}
