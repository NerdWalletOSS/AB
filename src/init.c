#ifdef XXX
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <sys/socket.h>
#include <curl/curl.h>
#include <netinet/in.h>
#endif
// #include <sys/uio.h> // if using sendmsg to send the datagram:
#include "ab_incs.h"
#include "ab_globals.h"
#include "auxil.h"
#include "spooky_hash.h"
#include "zero.h"
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

//<hdr>
int
init(
    void
    )
//</hdr>
{
  int status = 0;

  if ( AB_NUM_BINS > 32767 ) { go_BYE(-1); }

  g_halt = false;
  //-----------------------------------------------

  for ( uint32_t i = 0; i < AB_MAX_NUM_TESTS; i++ ) {
    zero_test(i);
  }

  // DO NOT CHANGE THE FOLLOWING VALUES for g_seed1 and g_seed2
  g_seed1 = 961748941; // large prime number
  g_seed2 = 982451653; // some other large primenumber
  spooky_init(&g_spooky_state, g_seed1, g_seed2);
  srand48(g_seed1);

  g_sz_ss_response = 2048;
  g_ss_response = malloc(g_sz_ss_response); // some initial start
  return_if_malloc_failed(g_ss_response);
  memset(g_ss_response, '\0', g_sz_ss_response);
  //-----------------------------------------------
  if ( g_sz_log_q > 0 ) { 
    g_log_q = malloc(g_sz_log_q * sizeof(PAYLOAD_TYPE)); 
    return_if_malloc_failed(g_log_q);
    memset(g_log_q, '\0', (g_sz_log_q * sizeof(PAYLOAD_TYPE)));
    g_n_log_q = 0;
  }

  if ( g_uuid_len < 1 ) { go_BYE(-1); }
  g_uuid = malloc((g_uuid_len+1));
  return_if_malloc_failed(g_uuid);
  memset(g_uuid, '\0',  g_uuid_len+1);

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
  // User sets proportions as percentages
  if ( AB_NUM_BINS < 100 ) { go_BYE(-1); }
  // For Lua
  g_L = luaL_newstate(); if ( g_L == NULL ) { go_BYE(-1); }
  luaL_openlibs(g_L);  
  if ( ( luaL_loadfile(g_L, "rc.lua") ) || 
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

#ifdef XXX
// Has been deprecated. Delete later on
int
make_guid(
    uint64_t start, /* input */
    uint8_t X[AB_GUID_LENGTH] /* output */
    )
{
  int status = 0; bool first_call;
  int letters[62] = { 
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'z' };
  if ( start == 0 ) { first_call = false; } else { first_call = true; }
  if ( first_call ) {
    uint8_t Y[AB_GUID_LENGTH];
    for ( int i = 0; i < AB_GUID_LENGTH; i++ ) { 
      uint8_t x = start & 0x3F;
      if ( x >= 64 ) { go_BYE(-1); }
      if ( x >= 62 ) { x = 61; }
      uint8_t letter = letters[x];
      Y[i] = letter;
      start = start >> 6;
    }
    for ( int i = 0; i < AB_GUID_LENGTH; i++ ) { 
      X[i] = Y[AB_GUID_LENGTH-1-i];
    }
    goto BYE;
  }
  int carry = 0;
  for ( int i = AB_GUID_LENGTH-1; i >= 0; i-- ) { 
    uint8_t c = X[i];
    c += carry;
    int r =  alpha_range(c); 
    // If incrementing is safe, do so
    if ( r > 0 ) { X[i] = c+1;  break;  }
    // Incrementing will cause non-alphanumeric character.
    // So, we need to deal with it appropriately.
    carry = 0;
    switch ( r ) { 
      case -1 : X[i] = 48; break; 
      case -2 : X[i] = 65; break; 
      case -3 : X[i] = 97; break; 
      case -4 : X[i] = 48; carry = 1; break; 
      default : go_BYE(-1); break;
    }
  }
BYE:
  return status;
}


int 
alpha_range(
    uint8_t x
    )
{
  /*
48 to 57 0..9
65 to 90 A..Z
97 to 122 a..z
*/
  if ( x < 48-1 ) { return -1; }
  if ( ( x >= 48-1 ) && ( x <= 57-1 ) )  { return 1; }
  if ( x < 65-1 ) { return -2; }
  if ( ( x >= 65-1 ) && ( x <= 90-1 ) )  { return 2; }
  if ( x < 97-1 ) { return -3; }
  if ( ( x >= 97-1 ) && ( x <= 122-1 ) )  { return 3; }
  return -4;
}
#endif

