/* DO NOT AUTO GENERATE HEADER FILE  */// T is used to measure individual times 
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "macros.h"
#include "auxil.h"

#define CHUNK_SIZE 16384
char g_chunk[CHUNK_SIZE];

extern int execute(
    CURL *ch,
    const char *url
    ) ;

int
execute(
    CURL *ch,
    const char *url
    )
{
  int status = 0; 
  CURLcode curl_res;
  long http_code;
  curl_easy_setopt(ch, CURLOPT_URL, url);
  curl_res = curl_easy_perform(ch);
  if ( curl_res < 0 ) { go_BYE(-1); }
  curl_easy_getinfo(ch, CURLINFO_RESPONSE_CODE, &http_code);
  // fprintf(stderr, "return = %s \n", g_chunk);
  for ( int i = 0; i < CHUNK_SIZE; i++ ) { g_chunk[i] = '\0'; }
BYE:
  return status;
}

static size_t 
WriteMemoryCallback(
    void *contents, 
    size_t size, 
    size_t nmemb, 
    void *userp
    )
{
  size_t realsize = size * nmemb;
  memcpy(userp, contents, realsize);
  // fprintf(stderr, "Copied %d bytes. First 32 bytes = ", (int)realsize);
  /*
  for ( int i = 0; i < 64; i++ ) { 
    fprintf(stderr, "%c", ((char *)userp)[i]);
  }
  fprintf(stderr, "\n");
  */
  return realsize;
}


int 
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  FILE *ofp = NULL; // for timing measurements
  CURL *ch = NULL;
  FILE *fp = NULL;
  CURLINFO info;
  long http_code;
  double c_length;  
#define MAX_LEN_SERVER_NAME 127
#define MAX_LEN_URL 127
  char server[MAX_LEN_SERVER_NAME+1];
  char url[MAX_LEN_URL+1];
  char base_url[MAX_LEN_SERVER_NAME+1+64]; 
  bool write_to_file = false;
  uint64_t t_start, t_stop;
  uint32_t *T = NULL; // for timing measurements

  if ( argc != 3 ) { go_BYE(-1); }

  memset(server, '\0', MAX_LEN_SERVER_NAME+1);
  strcpy(server, argv[1]); 

  int itemp; 
  status = stoI4(argv[2], &itemp); cBYE(status);
  if ( ( itemp < 80 ) || ( itemp >= 65536 ) ) { go_BYE(-1); }
  uint16_t ab_port = (uint16_t)itemp; 


  memset(g_chunk, '\0', CHUNK_SIZE);
  /*
   http://curl.haxx.se/libcurl/c/libcurl-tutorial.html.  When using
  the HTTP protocol, there are many different ways a client can
  provide those credentials to the server and you can control which
  way libcurl will (attempt to) use them. The default HTTP
  authentication method is called 'Basic', which is sending the name
  and password in clear-text in the HTTP request, base64-encoded. This
  is insecure.
  */

  printf("init curl session\n");
  ch = curl_easy_init();

   // follow redirects
  curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);
  // insecure is okay
  curl_easy_setopt(ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, 0);
  // authentication stuff
  // curl_easy_setopt(ch, CURLOPT_USERNAME, username);
  // curl_easy_setopt(ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
  // curl_easy_setopt(ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
  // curl_easy_setopt(ch, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST);
  // curl_easy_setopt(ch, CURLOPT_HTTPAUTH, CURLAUTH_GSSNEGOTIATE);

  /* send all data to this function  */ 
  curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  /* we pass our 'chunk' struct to the callback function */ 
  curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *)&g_chunk);

  int num_tests = 128;
  int niter = 4;
  int nU = 1048576; 
  int start_uuid = 12345678;
  char buf[1024];
  int test_id = 1;

  // Set base URL
  memset(base_url, '\0', (MAX_LEN_SERVER_NAME+1+64));
  sprintf(base_url, "%s:%d", server, ab_port);

  fprintf(stderr, "Reloading the ab server\n");
  sprintf(url, "%s:%d/Restart", server, ab_port);
  status = execute(ch, url); cBYE(status);
    curl_easy_getinfo(ch, CURLINFO_RESPONSE_CODE, &http_code);
  if ( http_code != 200 ) { 
    fprintf(stderr, "Unable to reload server\n");
  }
  else {
    fprintf(stderr, "Reloaded server\n");
  }

  fprintf(stderr, "Creating %d tests T1, T2, .. \n", num_tests);
  for ( int test_id = 0; test_id < num_tests; test_id++ ) { 
    sprintf(url, "%s:%d/AddFakeTest?TestName=T%d&TestType=ABTest&NumVariants=3", server, ab_port, test_id);
    execute(ch, url);
    curl_easy_getinfo(ch, CURLINFO_RESPONSE_CODE, &http_code);
    if ( http_code != 200 ) { 
      fprintf(stderr, "Unable to create test T%d\n", test_id); go_BYE(-1); 
    }
    else {
      fprintf(stderr, "Created test T%d\n", test_id); 
    }
  }

  // Get variants for these tests for nU users
  t_start = get_time_usec();
  int num_bad = 0; int ndot = 0;
  uint64_t maxt = 0, t1, t2, t;
  // T is used to measure individual times 
  T = malloc(sizeof(uint32_t) * (num_tests* niter * nU));
  return_if_malloc_failed(T);
  int tidx = 0; // index into T
  int ndots = 0;
  for ( int iter = 0; iter < niter; iter++ ) { 
    fprintf(stderr, "iter = %d \n", iter);
    for ( int test_id = 0; test_id < num_tests; test_id++ ) { 
      for ( int uid = 0; uid < nU; uid++ ) {
        sprintf(url, "%s/GetVariant?TestName=T%d&TestType=ABTest&UUID=%d", 
            base_url, test_id, start_uuid+uid);
        t1 = RDTSC();
        execute(ch, url);
        curl_easy_getinfo(ch, CURLINFO_RESPONSE_CODE, &http_code);
        if ( http_code != 200 ) { 
          continue; num_bad++; 
        }
        t2 = RDTSC();
        if ( t2 < t1 ) {  continue; }
        t = t2 - t1;
        T[tidx++] = t;
        if ( ( tidx % 500 ) == 0 ) { 
          ndots++; 
          fprintf(stderr, ".");
          if ( ndots >= 72 ) { 
            fprintf(stderr, "\n");
            ndots = 0;
          }
        }
      }
    }
  }
  t_stop = get_time_usec();
  double avg_time = 0;
  double min_time = INT_MAX;
  double max_time = INT_MIN;
  for ( int i = 0; i < tidx; i++ ) { 
    if ( T[i] > max_time ) { max_time = T[i]; }
    if ( T[i] < min_time ) { min_time = T[i]; }
    avg_time += T[i];
  }
  avg_time /= tidx;
  fprintf(stderr, "num_bad   \t = %d\n", num_bad);
  fprintf(stderr, "num_tests \t = %d\n", num_tests);
  fprintf(stderr, "num_trials\t = %d\n", tidx);
  fprintf(stderr, "num_users \t = %d\n", nU);
  fprintf(stderr, "avg_time  \t = %f\n",  avg_time);
  fprintf(stderr, "min_time  \t = %f\n",  min_time);
  fprintf(stderr, "max_time  \t = %f\n",  max_time);
  curl_easy_cleanup(ch); ch = NULL;

  ofp = fopen("_timings.bin", "wb");
  return_if_fopen_failed(ofp, "_timings.bin", "wb");
  fwrite(T, sizeof(uint32_t), tidx, ofp);
  fclose_if_non_null(ofp);
BYE:
  fclose_if_non_null(ofp);
  free_if_non_null(T);
  if ( ch != NULL ) { curl_easy_cleanup(ch); }
  return status ;
}
