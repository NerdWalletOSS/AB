#include "incs.h"
#include "setup_curl.h"

static size_t 
WriteMemoryCallback(
    void *contents, 
    size_t size, 
    size_t nmemb, 
    void *userp
    )
{
  size_t realsize = size * nmemb;
  return realsize;
}

int
setup_curl(
    CURL **ptr_ch
    )
{
  int status = 0;
  CURL *ch = NULL;

  ch = curl_easy_init();
  if ( ch == NULL ) { go_BYE(-1); }
   // follow redirects
  curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);
  // insecure is okay
  curl_easy_setopt(ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, 0);

  /* send all data to this function  */ 
  curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  *ptr_ch = ch;
BYE:
  return status;
}
