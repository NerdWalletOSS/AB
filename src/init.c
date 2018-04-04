#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "spooky_hash.h"
#include "zero_globals.h"
#include "init.h"
#include "ping_server.h"


size_t 
WriteMemoryCallback(
    void *contents, 
    size_t size, 
    size_t nmemb, 
    void *userp
    )
{
  int realsize = size * nmemb;
  if ( realsize > g_sz_ss_response ) { 
    for ( ; g_sz_ss_response < realsize; ) { 
      g_sz_ss_response *= 2 ;
    }
    g_ss_response = realloc(g_ss_response, g_sz_ss_response);
  }
  memcpy(userp, contents, realsize);
  return realsize;
}

#ifdef TEST_STATSD
  statsd_count(g_statsd_link, "count1", 123, 1.0);
  statsd_count(g_statsd_link, "count2", 125, 1.0);
  statsd_gauge(g_statsd_link, "speed", 10);
  statsd_timing(g_statsd_link, "request", 2400);
  sleep(1);
  statsd_inc(g_statsd_link, "count1", 1.0);
  statsd_dec(g_statsd_link, "count2", 1.0);
  int i;
  for (i=0; i<10; i++) {
      statsd_count(g_statsd_link, "count3", i, 0.8);
  }
#endif

int
init_lua(
    void
    )
{
  int status = 0;
  g_L = luaL_newstate(); if ( g_L == NULL ) { go_BYE(-1); }
  luaL_openlibs(g_L);  
  status = luaL_loadfile(g_L, "./ab.lua"); cBYE(status);
  status = lua_pcall(g_L, 0, 0, 0); cBYE(status);
BYE:
  return status;
}

int
setup_curl(
    const char *method,
    char *write_buffer,
    const char *server,
    int port,
    const char *url,
    const char *health_url,
    uint32_t timeout_ms,
    CURL **ptr_ch,
    struct curl_slist **ptr_curl_hdrs
)
{
  int status = 0;
  char *full_url = NULL;
  CURL *ch = NULL;
  struct curl_slist *curl_hdrs = NULL;
  CURLcode res;

  *ptr_ch = NULL;
  *ptr_curl_hdrs = NULL;


  ch = curl_easy_init();
  if ( ch == NULL ) { go_BYE(-1); }
  curl_hdrs = curl_slist_append(curl_hdrs, "Content-Type: application/json");
  curl_hdrs = curl_slist_append(curl_hdrs, "X-Caller-Client-ID: ab-runtime-service");
  /*
   * Robby Bryant 
   * fwiw - when you hit logger APIs, can send a custom header called
   * `X-Caller-Client-ID` with a a value of `ab-runtime-service` (or 
   * any other string of your choosing)?  that way we can slice out 
   * your traffic in our logs and metrics
   * */
  int len = strlen(server) + strlen(url) + 32;
  full_url = malloc(len); return_if_malloc_failed(full_url);
  int nw;
  if ( port <= 0 ) { 
    nw = snprintf(full_url, len-1, "http://%s/%s", server, url);
  }
  else {
    nw = snprintf(full_url, len-1, "http://%s:%d/%s", server,port,url);
  }
  if ( nw >= len-1 ) { go_BYE(-1); }
  res = curl_easy_setopt(ch, CURLOPT_URL, full_url);
  if ( res != CURLE_OK ) { go_BYE(-1);  }
  res = curl_easy_setopt(ch, CURLOPT_TIMEOUT_MS,     timeout_ms);
  if ( res != CURLE_OK ) { go_BYE(-1);  }
  res = curl_easy_setopt(ch, CURLOPT_DNS_CACHE_TIMEOUT,     100);
  if ( res != CURLE_OK ) { go_BYE(-1);  }
  if ( strcmp(method, "POST") == 0 ) { 
    res = curl_easy_setopt(ch, CURLOPT_POST, 1L);
    if ( res != CURLE_OK ) { go_BYE(-1);  }
  }
  else {
    res = curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    if ( res != CURLE_OK ) { go_BYE(-1);  }
    res = curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *)write_buffer);
    if ( res != CURLE_OK ) { go_BYE(-1);  }
  }
  /* set our custom set of headers */ 
  res = curl_easy_setopt(ch, CURLOPT_HTTPHEADER, curl_hdrs);
  if ( res != CURLE_OK ) { go_BYE(-1);  }
  //-------
  // Check that log server is listening
  if ( ( health_url != NULL ) && ( *health_url != '\0' ) ) {
    int ping_status = ping_server(server, port, health_url, NULL);
    if ( ping_status < 0 ) { 
      fprintf(stderr, "WARNING! Server %s:%d, url %s  not running\n", 
          server, port, health_url);
    }
    else {
      fprintf(stderr, "[INFO] Server %s:%d running.\n", server, port);
    }
  }
  *ptr_ch = ch;
  *ptr_curl_hdrs = curl_hdrs;
BYE:
  free_if_non_null(full_url);
  return status;
}

#ifdef TEST_INIT_STAND_ALONE
// TODO P2 Make a standalone Lua test out of this 
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char X[16];
  memset(X, '\0', 16);
  uint64_t t = get_time_usec();
  status = make_guid(t, X);
  fprintf(stderr, "%s\n", X);
  for( int i = 0; i < 40; i++ ) {
  status = make_guid(0, X);
  fprintf(stderr, "%s\n", X);
  }
BYE:
  return status;
}
#endif
