#include "incs.h"
#include "execute.h"

int
execute(
    CURL *ch,
    const char *url,
    long *ptr_http_code
    )
{
  int status = 0; 
  CURLcode curl_res;
  
  curl_easy_setopt(ch, CURLOPT_URL, url);
  curl_res = curl_easy_perform(ch);
  if ( curl_res < 0 ) { go_BYE(-1); }
  curl_easy_getinfo(ch, CURLINFO_RESPONSE_CODE, ptr_http_code);
BYE:
  return status;
}
