#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "ab_constants.h"
#include "ab_types.h"
#include "ab_globals.h"
#include "macros.h"
#include "auxil.h"
#include "ping_server.h"
//<hdr>
int 
ping_server(
    const char *server,
    int port,
    const char *url,
    double *ptr_time
    )
//</hdr>
{
  int status = 0;
  CURL *ch = NULL; CURLcode curl_res;
  char *buffer = NULL; int buflen = 0;
  long http_code;

  *ptr_time = 0;
  bool is_server = true;
  if ( ( server == NULL ) || ( *server == '\0' ) )  {
    is_server = false;
  }
  if ( ( port <= 0 ) || ( port >= 65536 ) ) { 
    is_server = false;
  }
  if ( ( url == NULL ) || ( *url == '\0' ) )  {
    is_server = false;
  }
  if ( is_server == false ) { 
    fprintf(stderr, "log server not identified. Not checking\n");
    goto BYE;
  }
  ch = curl_easy_init();
  buflen = strlen(server) + strlen(url) + 32;
  buffer = malloc(buflen * sizeof(char));
  return_if_malloc_failed(buffer);

  int nw = snprintf(buffer, buflen, "http://%s:%d/%s", server, port, url);
  if ( nw >= buflen ) { go_BYE(-1); }
  curl_easy_setopt(ch, CURLOPT_URL, buffer);
  curl_easy_setopt(ch, CURLOPT_TIMEOUT_MS, 1000);
  curl_res = curl_easy_perform(ch);
  if ( curl_res != CURLE_OK ) { 
    fprintf(stderr, "Failure at [%s] \n", buffer);
    go_BYE(-1); 
  }
  curl_easy_getinfo(ch, CURLINFO_RESPONSE_CODE, &http_code);
  if ( http_code != 200 ) { 
    fprintf(stderr, "Failure at [%s] \n", buffer);
    go_BYE(-1); 
  }
  curl_easy_getinfo(ch, CURLINFO_TOTAL_TIME, ptr_time);
BYE:
  free_if_non_null(buffer);
  curl_easy_cleanup(ch); 
  return status;
}
