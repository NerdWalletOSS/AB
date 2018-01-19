#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "spooky_hash.h"
#include "zero_globals.h"
#include "init.h"
#include "ping_server.h"

#include "get_variant.h"

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

int 
set_seed(
    const char *str,
    uint64_t *ptr_seed
    )
{
  int status = 0;
  uint64_t seed = 0;
  for ( unsigned int i = 0; i < strlen(str); i++ ) { 
    char c = str[i];
    unsigned int x;
    if ( ( c >= 'a' )  && ( c <='z' ) ) { 
      x = 10 + ( c - 'a' );
    }
    else if ( ( c >= '0' )  && ( c <='9' ) ) { 
      x = c - '0';
    }
    else {
      go_BYE(-1); 
    }
    seed |= x;
    seed *= 36;
  }
  *ptr_seed = seed;
BYE:
  return status;
}

int
init()
{
  int status = 0;
  if ( g_sz_log_q > 0 ) { 
    g_log_q = malloc(g_sz_log_q * sizeof(PAYLOAD_TYPE)); 
    return_if_malloc_failed(g_log_q);
    memset(g_log_q, '\0', (g_sz_log_q * sizeof(PAYLOAD_TYPE)));
    g_n_log_q = 0;
  }

  //---------------------------------------------------
  CURLcode res;
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if ( res != CURLE_OK) {
      fprintf(stderr, "curl_global_init() failed: %s\n",
          curl_easy_strerror(res));
      go_BYE(-1); 
    }

  // Following for statsd
  if ( ( *g_statsd_host == '\0' ) || ( *g_statsd_host == '\0' ) ) {
    fprintf(stderr, "WARNING! Not logging to statsd \n");
  }
  else {
    chk_not_null_str(g_statsd_inc);
    chk_not_null_str(g_statsd_timing);
    g_statsd_link = statsd_init(g_statsd_host, g_statsd_port);
    if ( g_statsd_link == NULL ) { go_BYE(-1); }
    // NOTE: Currently settin sample rate to 1.0 hard coded.
    // Change this if it becomes a performance problem
  }
  // Following for curl for logging GetVariant 
  if ( ( *g_log_server == '\0' ) || ( *g_log_url == '\0' ) ||
      ( ( g_log_port <= 1 )  || ( g_log_port >= 65535 ) ) ) {
    fprintf(stderr, "WARNING! /GetVariant NOT being logged\n");
  }
  else {
    status = setup_curl("POST", NULL, g_log_server, g_log_port, 
        g_log_url, g_log_health_url, 1000, &g_ch, &g_curl_hdrs);
    cBYE(status);
  }
  // Following for curl  for Session Service
  if ( ( *g_ss_server == '\0' ) || ( *g_ss_url == '\0' ) ||
      ( ( g_ss_port <= 1 )  || ( g_ss_port >= 65535 ) ) ) {
    fprintf(stderr, "WARNING! SessionService not in use\n");
  }
  else {
    /* The get_or_create endpoint averages around 25ms response, with 
     * a 95th percentile of 30ms. The plain get endpoint would be 
     * expected to be faster -- Andrew Hollenbach */
    status = setup_curl("GET", g_ss_response, g_ss_server, g_ss_port, 
        g_ss_url, g_ss_health_url, 50, &g_ss_ch, &g_ss_curl_hdrs);
    cBYE(status);
  }
  // For Lua
  g_L = luaL_newstate(); if ( g_L == NULL ) { go_BYE(-1); }
  luaL_openlibs(g_L);  
  if ( ( luaL_loadfile(g_L, "ab.lua") ) || 
      ( lua_pcall(g_L, 0, 0, 0)) )   {
    go_BYE(-1);
  }

BYE:
  if ( status < 0 ) { 
    free_globals();
  }
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
    double ping_time = 0;
    int ping_status = ping_server(server, port, health_url, &ping_time);
    if ( ping_status < 0 ) { 
      fprintf(stderr, "WARNING! Server %s:%d, url %s  not running\n", 
          server, port, health_url);
      shutdown_curl();
    }
    else {
      fprintf(stderr, "[INFO] Server %s:%d running. Time = %lf\n", 
          server, port, ping_time);
    }
  }
  *ptr_ch = ch;
  *ptr_curl_hdrs = curl_hdrs;
BYE:
  free_if_non_null(full_url);
  return status;
}

#ifdef TEST_INIT_STAND_ALONE
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
